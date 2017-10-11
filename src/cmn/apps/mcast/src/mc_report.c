#include "mc_core.h"
#include "mc_fdb.h"
#include "mc_grp.h"
#include "mc_src.h"
#include "mc_test.h"
#include "mc_parser.h"
#include "mc_util.h"
#include "mc_mgmt.h"
#include "mc_adapter.h"
#ifdef HAVE_MC_REPORT_V3_TO_V2
#include "lwip/inet.h"
#endif

/* 
 * Reception of Current-State Records
 
   Router State     Report Rec'd        New Router State         Actions   
   ------------   ------------  ----------------         -------   
   INCLUDE (A)    IS_IN (B)             INCLUDE (A+B)            (B)=GMI   
   INCLUDE (A)    IS_EX (B)             EXCLUDE (A*B,B-A)      (B-A)=0                                                         
                                                                                    Delete (A-B)                                                         
                                                                                    Group Timer=GMI   
   EXCLUDE (X,Y)  IS_IN (A)            EXCLUDE (X+A,Y-A)      (A)=GMI   
   EXCLUDE (X,Y)  IS_EX (A)            EXCLUDE (A-Y,Y*A)      (A-X-Y)=GMI                                                         
                                                                                    Delete (X-A)                                                         
                                                                                    Delete (Y-A)                                                         
                                                                                    Group Timer=GMI

*/                                                                                    



cs_boolean mc_record_isin_handle(
    mc_port_group_state_list_t *port_state, 
    mc_group_record_list_t *record)
{
   
    cs_uint16 member_interval = 0;
    mc_group_node_t  *grp_state = NULL;
    mc_node_t  *mc_node = NULL;

    cs_boolean update_flag = FALSE;

    grp_state = port_state->entry.grp_father;
    mc_node = grp_state->entry.mc_node;
    member_interval = mc_node->member_interval;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_isin_handle]\n");

    /* 6.4.1 */
    if(port_state->entry.filter_mode == SRC_INCLUDE) {  /* router state: IN(A), IS_IN(B) */  
        /*
         * new state: IN(A+B)
         * action: (B) = GMI
         */
         
        cs_list  *A = &port_state->entry.src_list;
        cs_list  *B = &record->entry.src_list;

        if(mc_src_cmp(A, B) < 0) {  /* B out of A, need to update fdb */
            update_flag = TRUE;
        }

        /* (B) = GMI */
        mc_x_mc_timer_set(B , member_interval, MC_TIMER_START_FLAG);

        /* A = A+B */
        mc_x_union_subset(A, B);

    }
    else {  /* router state: EX(X, Y), IS_IN(A) */

        /* 
         * new state: EX(X+A, Y-A)
         * action: (A) = GMI
         */
         
        cs_list *X = &port_state->entry.src_list;
        cs_list *Y = &port_state->entry.aged_src_list;
        cs_list *A = &record->entry.src_list;    

        /* (A) = GMI */
        mc_x_mc_timer_set(A, member_interval, MC_TIMER_START_FLAG);

        if(mc_src_cmp(X, A) < 0 ||  /* A is out of X */
            mc_src_find_intersection(Y, A)) {   /* Y and A is overlap */
            update_flag = TRUE;
        }
        
        /* X = X+A */
        mc_x_union_subset(X , A);

        /* Y = Y-A */
        mc_x_minus_subset(Y, A);
       
    }
    
    return update_flag;

}

cs_boolean mc_record_isex_handle(
    mc_port_group_state_list_t * port_state, 
    mc_group_record_list_t * record)
{
    cs_boolean update_flag = FALSE;
    
    cs_uint16 member_interval = 0;
    mc_group_node_t  *grp_state = NULL;
    mc_node_t      *mc_node = NULL;
    
    grp_state = port_state->entry.grp_father;
    mc_node = grp_state->entry.mc_node;
    member_interval = mc_node->member_interval;   

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_isex_handle]\n");

    /* 6.4.1 */
    if (port_state->entry.filter_mode == SRC_INCLUDE) {   /* router state: IN(A), IS_EX(B) */
        /*
         * new state : EX(A*B, B-A)
         * action: (B-A) = 0, Delete (A-B), Group Timer = GMI
         *
         */
         
        cs_list *A = &port_state->entry.src_list;
        cs_list *B = &record->entry.src_list;

        update_flag = TRUE;

        /* (B-A) = 0 */
        mc_x_minus_y(B, A ,&port_state->entry.aged_src_list);
        mc_x_mc_timer_set(&port_state->entry.aged_src_list , 0, MC_TIMER_STOP_FLAG);

        /* A = A*B */
        mc_x_intersection_subset(A, B);
    }
    else {  /* router state: EX(X, Y), IS_EX(A) */
        /*
         * new state: EX(A-Y, Y*A)
         * action: (A-X-Y) = GMI, Delete (X-A) and (Y-A), Group Timer = GMI
         */
         
        cs_list *X = &port_state->entry.src_list;
        cs_list *Y = &port_state->entry.aged_src_list;
        cs_list *A = &record->entry.src_list;

        cs_list  temp0_list;
        cs_list  temp1_list;

        cs_list  old_X;
        cs_list  old_Y;
 
        cs_lst_init(&temp0_list,  NULL);   
        cs_lst_init(&temp1_list,  NULL); 

        cs_lst_init(&old_X,  NULL);   
        cs_lst_init(&old_Y,  NULL); 

        mc_x_set_y(&old_X, X);
        mc_x_set_y(&old_Y, Y);

        /* (A-X-Y) = GMI*/
        mc_x_minus_y(A, Y, &temp0_list);           
        mc_x_minus_y(&temp0_list, X, &temp1_list); 
        mc_x_mc_timer_set(&temp1_list , member_interval, MC_TIMER_START_FLAG);  

        /* Delete (X-A) */
        mc_src_list_clear(&temp0_list);
        mc_x_minus_y(X, A, &temp0_list);            /* temp0_list = X-A */
        mc_x_minus_subset (X, &temp0_list);     /* X = X-(X-A) */


        /* EXCLUDE (A-Y,Y*A) */
        mc_x_union_subset(X, &temp1_list);      /* X = X-(X-A) + (A-X-Y) = A-Y */
       
        mc_x_intersection_subset(Y, A);         /* Y = Y*A */

        if(mc_src_cmp(X, &old_X) || mc_src_cmp(Y, &old_Y)) {
            update_flag = TRUE;
        }

        mc_src_list_clear(&temp0_list);
        mc_src_list_clear(&temp1_list);
        mc_src_list_clear(&old_X);
        mc_src_list_clear(&old_Y);
    }

    /* Group Timer = GMI */
    port_state->entry.filter_mode = SRC_EXCLUDE;
    mc_timer_set(&port_state->entry.grp_timer, member_interval , MC_TIMER_START_FLAG);        

    return update_flag;
    
}

/* 
 * Reception of State-Change Records
    Router State      Report Rec'd    New Router State          Actions
    ------------   ------------ ----------------        -------

    INCLUDE (A)      ALLOW (B)      INCLUDE (A+B)             (B)=GMI

    INCLUDE (A)      BLOCK (B)       INCLUDE (A)                 Send Q(G,A*B)

    INCLUDE (A)      TO_EX (B)       EXCLUDE (A*B,B-A)       (B-A)=0
                                                                                   Delete (A-B)
                                                                                   Send Q(G,A*B)
                                                                                   Group Timer=GMI

    INCLUDE (A)      TO_IN (B)        INCLUDE (A+B)             (B)=GMI
                                                                                    Send Q(G,A-B)

    EXCLUDE (X,Y)   ALLOW (A)       EXCLUDE (X+A,Y-A)       (A)=GMI

    EXCLUDE (X,Y)   BLOCK (A)        EXCLUDE (X+(A-Y),Y)     (A-X-Y)=Group Timer
                                                                                     Send Q(G,A-Y)

    EXCLUDE (X,Y)   TO_EX (A)        EXCLUDE (A-Y,Y*A)        (A-X-Y)=Group Timer
                                                                                      Delete (X-A)
                                                                                      Delete (Y-A)
                                                                                      Send Q(G,A-Y)
                                                                                      Group Timer=GMI
  */


cs_boolean mc_record_toin_handle(
    mc_port_group_state_list_t * port_state, 
    mc_group_record_list_t * record)
{    
    mc_group_node_t  *grp_state = NULL;
    mc_node_t  *mc_node = NULL;
    mc_port_t   *port = NULL;

    cs_uint16 member_interval = 0;
    cs_uint32 last_member_query_time = 0;
    cs_boolean update_flag = FALSE;
    
    grp_state = port_state->entry.grp_father;
    mc_node = grp_state->entry.mc_node;    
    port = port_state->entry.port;
    
    member_interval = mc_node->member_interval;
    last_member_query_time = 
                mc_node->last_member_query_interval * mc_node->last_member_query_count;

   MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_toin_handle]\n");
    
    /* 6.4.2 */
    if(port_state->entry.filter_mode == SRC_INCLUDE) {  /* router state: IN(A), TO_IN(B) */
        /*
         * non-fast-leave:
         * new state: IN(A+B)
         * action: (B) = GMI, lower (A-B)
         *
         * fast-leave:
         * new state: IN(B)
         * action: (B) = GMI, delete (A-B)
         */

        cs_list *A = &port_state->entry.src_list;
        cs_list *B = &record->entry.src_list;     

        /*(B)=GMI*/
        mc_x_mc_timer_set(B, member_interval, MC_TIMER_START_FLAG);

        if(PORT_NON_FAST_LEAVE == port->leave_mode) {
            if(mc_src_cmp(A, B) < 0) {  /* B is out of A */
                update_flag = TRUE;
            }
            
            mc_x_mc_timer_set(A, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);
            mc_x_union_subset(A,B);
        }
        else {
            if(mc_src_cmp(A, B)) {  /* A != B */
                update_flag = TRUE;
            }
            
            mc_x_set_y(A, B);
        }

    }
    else {  /* router state: EX(X, Y), TO_IN(A) */
        /*
         * non-fast-leave:
         * new state: EX(X+A, Y-A)
         * action: (A) = GMI, lower (X-A), lower Group Timer
         *
         * fast-leave:
         * new state: IN(A)
         * action: (A) = GMI, Delete (X-A) and Y
         */
         
        cs_list *X = &port_state->entry.src_list;
        cs_list *Y = &port_state->entry.aged_src_list;
        cs_list *A = &record->entry.src_list;        

        /* (A)=GMI */
        mc_x_mc_timer_set(A, member_interval, MC_TIMER_START_FLAG);

        if(PORT_NON_FAST_LEAVE == port->leave_mode) {

            if(mc_src_cmp(X, A) < 0 /* A is out of X */
                || mc_src_find_intersection(Y, A)) {  /* Y and A is overlap */
                update_flag = TRUE;
            }
            
            /* lower (X-A), X = X+A */
            mc_x_mc_timer_set(X, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);
            mc_x_union_subset(X, A);            

            /* Y = Y -A */
            mc_x_minus_subset(Y,A); 
            
            /* lower group timer */
            mc_timer_set(&port_state->entry.grp_timer, 
                    last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);
        }
        else {
            mc_x_set_y(X,A);
            mc_x_clear(Y);

            port_state->entry.filter_mode = SRC_INCLUDE;
            mc_timer_set(&port_state->entry.grp_timer, 
                    0, MC_TIMER_STOP_FLAG); 
            
            update_flag = TRUE;
        }

    }
    
    return update_flag;

}



cs_boolean mc_record_toex_handle( 
    mc_port_group_state_list_t * port_state, 
    mc_group_record_list_t * record)
{
    mc_node_t   *mc_node = NULL;
    mc_group_node_t  *grp_state = NULL;
    mc_port_t *port = NULL;

    cs_uint16 member_interval = 0;
    cs_uint32 last_member_query_time = 0;

    cs_boolean update_flag = FALSE;

    grp_state = port_state->entry.grp_father; 
    mc_node = grp_state->entry.mc_node;
    port = port_state->entry.port;

    member_interval = mc_node->member_interval;
    last_member_query_time = 
                mc_node->last_member_query_interval * mc_node->last_member_query_count;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_toex_handle]\n");
    
    /* 6.4.2 */
    if(port_state->entry.filter_mode == SRC_INCLUDE) {  /* router state: IN(A), TO_EX(B) */
        /*
         * non-fast-leave
         * new state: EX(A*B, B-A)
         * action: (B-A) = 0, Delete (A-B), lower (A*B), Group Timer = GMI
         *
         * fast-leave
         * new state: EX(0, B)
         * action: Delete (A), (B) = 0, Group Timer = GMI
         */        
        
        cs_list *A = &port_state->entry.src_list;
        cs_list *B = &record->entry.src_list;                        

        if(PORT_NON_FAST_LEAVE == port->leave_mode) {
            cs_list A_B;

            cs_lst_init(&A_B, NULL);
            
            mc_x_minus_y(A , B, &A_B);
            /* A = A*B = A-(A-B), delete (A-B) */
            mc_x_minus_subset(A, &A_B);  
            mc_src_list_clear(&A_B);
            
            /* lower (A*B) */
            mc_x_mc_timer_set(A, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);

            /* (B-A) = 0 */
            mc_x_minus_y(B, A, &port_state->entry.aged_src_list);
            mc_x_mc_timer_set(&port_state->entry.aged_src_list, 0, MC_TIMER_STOP_FLAG);
        }
        else {
            /* delete (A) */
            mc_x_clear(A);

            /* (B) = 0 */
            mc_x_set_y(&port_state->entry.aged_src_list ,B);
            mc_x_mc_timer_set(&port_state->entry.aged_src_list, 0, MC_TIMER_STOP_FLAG);
        }

        port_state->entry.filter_mode = SRC_EXCLUDE;
        mc_timer_set(&port_state->entry.grp_timer, member_interval, MC_TIMER_START_FLAG);
        
        update_flag = TRUE;
    }
    else {  /* router state: EX(X, Y), TO_EX(A) */
        /*
         * non-fast-leave:
         * new state: EX(A-Y, Y*A)
         * action: Delete (X-A) and (Y-A), lower (A-Y), Group Timer = GMI
         *
         * fast-leave:
         * new state: EX(0, A)
         * action: Deleter (X) and (Y), (A) = 0, Group Timer = GMI
         */
    
        cs_list *X = &port_state->entry.src_list;
        cs_list *Y = &port_state->entry.aged_src_list;
        cs_list *A = &record->entry.src_list;        
        
        if(PORT_NON_FAST_LEAVE == port->leave_mode) {
            cs_list A_Y;

            cs_lst_init(&A_Y, NULL);       

            mc_x_minus_y(A, Y, &A_Y);

            if(mc_src_cmp(X, &A_Y) || mc_src_cmp(Y, A)) {   /* X != (A-Y) or Y != A */
                update_flag = TRUE;
            }        

            /* X = A - Y */
            mc_x_set_y(X, &A_Y);
            mc_x_mc_timer_set(X, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);

            /* Y = Y*A, delete (Y-A) */
            mc_x_intersection_subset(Y, A);

            mc_src_list_clear(&A_Y);
        }
        else {
            if(cs_lst_count(X) || mc_src_cmp(Y, A)) {   /* X is not empty or Y != A */
                update_flag = TRUE;
            }
            
            /* delete (X) */
            mc_x_clear(X);

            /* Y = A, (A) = 0 */
            mc_x_set_y(Y,A);
            mc_x_mc_timer_set(&port_state->entry.aged_src_list, 0, MC_TIMER_STOP_FLAG);
        }

        /* Group Timer = GMI */
        mc_timer_set(&port_state->entry.grp_timer, member_interval, MC_TIMER_START_FLAG);
        
    }
    
    return update_flag;
}

cs_boolean mc_record_allow_handle( 
    mc_port_group_state_list_t *port_state, 
    mc_group_record_list_t *record)
{    
    mc_group_node_t  *grp_state = NULL;
    mc_node_t      *mc_node = NULL;

    cs_uint16 member_interval = 0;

    cs_boolean update_flag = FALSE;

    grp_state = port_state->entry.grp_father;
    mc_node = grp_state->entry.mc_node;
    
    member_interval = mc_node->member_interval;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_allow_handle]\n");

    /* 6.4.2 */
    if(port_state->entry.filter_mode == SRC_INCLUDE) {  /* router state: IN(A), ALLOW(B) */   
        /*
         * new state: IN(A+B)
         * action: (B) = GMI
         */
                
        cs_list *A = &port_state->entry.src_list;
        cs_list *B = &record->entry.src_list;

        if(mc_src_cmp(A, B) < 0) {  /* B is out of A */
            update_flag = TRUE;
        }

        /* (B) = GMI */
        mc_x_mc_timer_set(B, member_interval, MC_TIMER_START_FLAG);

        /* A = A+B */
        mc_x_union_subset(A, B);
    }
    else {  /* router state: EX(X, Y), ALLOW(A) */
        /*
         * new state: EX(X+A, Y-A)
         * action: (A) = GMI
         */
        
        cs_list *X=&port_state->entry.src_list;
        cs_list *Y =&port_state->entry.aged_src_list;
        cs_list *A=&record->entry.src_list;

        if(mc_src_cmp(X, A) < 0 /* A is out of X */
            || mc_src_find_intersection(Y, A))  {   /* Y and A is overlap */
            update_flag = TRUE;
        }

         /*(A)=GMI*/
        mc_x_mc_timer_set(A, member_interval, MC_TIMER_START_FLAG);

        /*X+A*/
       mc_x_union_subset(X, A);         

        /* Y = Y-A */
        mc_x_minus_subset(Y, A);
    }

    return update_flag;
    
}


cs_boolean mc_record_block_handle( 
    mc_port_group_state_list_t *port_state, 
    mc_group_record_list_t * record)
{       
    mc_group_node_t  *grp_state = NULL;
    mc_node_t *mc_node = NULL;
    mc_port_t  *port = NULL;

    cs_uint16 member_interval = 0;
    cs_uint32 last_member_query_time = 0;
     
    cs_boolean update_flag = FALSE;
    
    grp_state = port_state->entry.grp_father;
    mc_node = grp_state->entry.mc_node;
    port = port_state->entry.port;
    
    member_interval = mc_node->member_interval;
    last_member_query_time = 
                mc_node->last_member_query_interval * mc_node->last_member_query_count;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_record_block_handle]\n");
    
    /* 6.4.2 */
    if(port_state->entry.filter_mode == SRC_INCLUDE) {  /* router state: IN(A), BLOCK(B) */
        /*
         * non-fast-leave:
         * new state: IN(A)
         * action: lower (A*B)
         *
         * fast-leave:
         * new state: IN(A-B)
         * action: Delete (A*B)
         */
         
        cs_list *A = &port_state->entry.src_list;
        cs_list *B = &record->entry.src_list;

        if(PORT_NON_FAST_LEAVE == port->leave_mode) {
            
            cs_list AxB;

            cs_lst_init(&AxB, NULL);

            /* lower (A*B) */
            mc_x_intersection_y(A, B, &AxB);
            mc_x_mc_timer_set(&AxB, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);
            mc_x_union_subset(A, &AxB);

            mc_src_list_clear(&AxB);
        }
        else {
            if(mc_src_find_intersection(A, B)) {    /* A and B is overlap */
                update_flag = TRUE;
            }

            /* A = A -B */
            mc_x_minus_subset(A, B);
        }
    }
    else {  /* router state: EX(X, Y), BLOCK(A) */
        /*
         * non-fast-leave:
         * new state: EX(X+(A-Y), Y)
         * action: lower (A-Y)
         *
         * fast-leave:
         * new state: EX(X-A, Y+A)
         * action: (X*A) = 0
         */
         
        cs_list *A = &record->entry.src_list;
        cs_list *X = &port_state->entry.src_list;
        cs_list *Y = &port_state->entry.aged_src_list;

        if(PORT_NON_FAST_LEAVE == port->leave_mode) {
            cs_list A_Y;

            cs_lst_init(&A_Y, NULL);

            mc_x_minus_y(A, Y, &A_Y);

            if(mc_src_cmp(X, &A_Y) < 0) {   /* (A-Y) is out of X */
                update_flag = TRUE;
            }

            /* lower (A-Y) */
            mc_x_mc_timer_set(&A_Y, last_member_query_time, MC_TIMER_DELAY_STOP_FLAG);

            /* X = X + (A-Y) */
            mc_x_union_subset(X, &A_Y);
        }
        else {
            if(mc_src_find_intersection(X, A)   /* X and A is overlap */
                || mc_src_cmp(Y, A) < 0) {  /* A is out of Y */
                update_flag = TRUE;
            }
            
            /* X = X - A */
            mc_x_minus_subset(X, A);

            /* Y = Y + A */
            mc_x_union_subset(Y, A);
        }

    }

    return update_flag;
    
}


cs_boolean mc_port_grp_update(mc_port_group_state_list_t *port_grp, mc_group_record_list_t *record)
{
    cs_boolean update_flag = FALSE;
    
    switch(record->entry.record_type) {
        case MODE_IS_INCLUDE:
            update_flag = mc_record_isin_handle(port_grp, record);
            break;

        case MODE_IS_EXCLUDE:
            update_flag = mc_record_isex_handle(port_grp, record);
            break;

        case CHANGE_TO_INCLUDE_MODE:
            update_flag = mc_record_toin_handle(port_grp, record);
            break;

        case CHANGE_TO_EXCLUDE_MODE:
            update_flag = mc_record_toex_handle(port_grp, record);
            break;

        case ALLOW_NEW_SOURCES:
            update_flag = mc_record_allow_handle(port_grp, record);
            break;

        case BLOCK_OLD_SOURCES:
            update_flag = mc_record_block_handle(port_grp, record);
            break;

        default :
            break;
    }

    return update_flag;
}

cs_status mc_update_port_grp_entry(
    mc_port_group_state_list_t *port_grp, 
    mc_group_record_list_t *record)
{
    mc_port_group_state_list_t *new_port_grp = NULL;    

    cs_status ret = CS_E_OK;

    if(IGMP_V1_REPORT == record->entry.pdu_type ||
       IGMP_V2_REPORT == record->entry.pdu_type ||
       MLD_V1_REPORT == record->entry.pdu_type) {
           cs_uint64 update_time = cs_current_time();
           if(update_time < port_grp->entry.update_time + 1000) {
               MC_LOG(IROS_LOG_LEVEL_INF, "report too fast!\n");
               return CS_E_ERROR;
           }
           else {
                MC_LOG(IROS_LOG_LEVEL_INF, "upate grp time\n");
                port_grp->entry.update_time = update_time;
           }
    }

    new_port_grp = mc_port_grp_create();
    if(NULL == new_port_grp) {
        return CS_E_RESOURCE;
    }
    
    mc_port_grp_copy(new_port_grp, port_grp);

    MC_UPDATE_PORT_GRP_DUMP(port_grp, record);

    if(mc_port_grp_update(new_port_grp, record)) {  /* update hw fdb */
        
        if(mc_port_grp_exist(new_port_grp)) {   /* update port grp */
           
            ret = mc_port_fdb_entry_update(port_grp, new_port_grp);
            if(CS_E_OK == ret) {
                mc_port_grp_copy(port_grp, new_port_grp);
            }
        }
        else {  /* del port grp */
            ret = mc_port_fdb_entry_del(port_grp);

            if(CS_E_OK == ret) {
                mc_group_node_t *grp = new_port_grp->entry.grp_father;                

                if(!mc_grp_exist(grp)) {
                    cs_hash_tbl * grp_table = NULL;
                    mc_node_t *mc_node = grp->entry.mc_node;
                    grp_table = mc_node->grp_tbl[grp->entry.vlanid];
                    mc_grp_rm(grp_table, grp);
                    mc_grp_destroy(grp);
                }
            }
        }
    }
    else {  /* only update timer */
        mc_group_timer_update(port_grp, new_port_grp);
    }

    mc_port_grp_destroy(new_port_grp);

    return ret;
    
}

cs_status mc_add_isex_record(mc_port_group_state_t *port_grp_entry, mc_group_record_t *record)
{
    mc_node_t *mc_node = port_grp_entry->grp_father->entry.mc_node;
    
    port_grp_entry->filter_mode = SRC_EXCLUDE;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_add_isex_record]\n");

    /* set grp timer */
    mc_timer_set(&port_grp_entry->grp_timer, mc_node->member_interval, MC_TIMER_START_FLAG);

    if(cs_lst_count(&record->src_list)) {
        cs_list *src_list = &port_grp_entry->aged_src_list;

        mc_src_list_copy(src_list, &record->src_list);
        mc_src_mc_timer_set(src_list, 0, MC_TIMER_STOP_FLAG);
    }

    return CS_E_OK;
}

cs_status mc_add_isin_record(mc_port_group_state_t *port_grp_entry, mc_group_record_t *record)
{   
    port_grp_entry->filter_mode = SRC_INCLUDE;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_add_isin_record]\n");

    /* set grp timer */
    mc_timer_set(&port_grp_entry->grp_timer, 0, MC_TIMER_STOP_FLAG);

    if(cs_lst_count(&record->src_list)) {
        cs_list *src_list = &port_grp_entry->src_list;
        mc_node_t *mc_node = port_grp_entry->grp_father->entry.mc_node;

        mc_src_list_copy(src_list, &record->src_list);
        mc_src_mc_timer_set(src_list, mc_node->member_interval, MC_TIMER_START_FLAG);

        return CS_E_OK;
    }
    else {  /* no src, it is an illegal report */
        return CS_E_ERROR;
    }
}

cs_status mc_add_port_grp_entry(mc_group_node_t *grp, mc_port_t *port, mc_group_record_t *record)
{
    mc_port_group_state_list_t *port_state = NULL;
    mc_node_t *mc_node = NULL;
    

    cs_status ret = CS_E_OK;

    if(BLOCK_OLD_SOURCES == record->record_type) {
        return CS_E_NOT_FOUND;
    }
    else if(CHANGE_TO_INCLUDE_MODE == record->record_type && 0 == record->src_list.count) {
        return CS_E_NOT_FOUND;
    }
    
    if(CHANGE_TO_INCLUDE_MODE == record->record_type ||
        ALLOW_NEW_SOURCES == record->record_type) {
        record->record_type = MODE_IS_INCLUDE;
    }
    else if(CHANGE_TO_EXCLUDE_MODE == record->record_type) {
        record->record_type = MODE_IS_EXCLUDE;
    }

    mc_node = (mc_node_t *)grp->entry.mc_node;

    if(!record->flag && mc_node->mode == MC_MANUAL) {
        return ret;
    }
    
    if(port->grp_count == port->max_grp) {
        return CS_E_RESOURCE;
    }

    MC_ADD_PORT_GRP_DUMP(port, record);

    port_state = mc_port_grp_create();
    if(NULL == port_state) {
        MC_LOG(IROS_LOG_LEVEL_CRI, "creat port grp entry fail!\n");
        return CS_E_RESOURCE;
    }

    port_state->entry.grp_father = grp;
    port_state->entry.port = port;  
    if(MODE_IS_INCLUDE == record->record_type) {    /* in mode */
        mc_add_isin_record(&port_state->entry, record);
    }
    else {  /* ex mode */
        mc_add_isex_record(&port_state->entry, record);
    }       

    mc_timer_set_static(&port_state->entry.grp_timer, record->flag);
    /* TODO: set src static flag */
        
    ret = mc_port_fdb_entry_add(port_state);
    if(ret) {   /* rollback */
        MC_LOG(IROS_LOG_LEVEL_MAJ, "add port grp fail(err: %d)!\n", ret);
        mc_port_grp_destroy(port_state);
        return ret;
    }

    if(IGMP_V1_REPORT == record->pdu_type ||
       IGMP_V2_REPORT == record->pdu_type ||
       MLD_V1_REPORT == record->pdu_type) {
       port_state->entry.update_time = cs_current_time();
    }

    mc_port_grp_add(grp, port_state);

    return ret;
}

cs_status mc_new_port_grp_entry(
    mc_node_t *mc_node, 
    mc_port_t *port, 
    mc_group_record_t *record)
{
    cs_hash_tbl *grp_table = NULL;
    mc_group_node_t *grp_state = NULL;

    cs_status ret = CS_E_OK;
    
    cs_uint16 vid = 0;

    if(BLOCK_OLD_SOURCES == record->record_type) {
        return CS_E_NOT_FOUND;
    }
    else if(CHANGE_TO_INCLUDE_MODE == record->record_type && 0 == record->src_list.count) {
        return CS_E_NOT_FOUND;
    }

    if(!record->flag && mc_node->mode == MC_MANUAL) {
        return ret;
    }
    
    if(mc_node->cur_grp_num == mc_node->max_grp_num
        || port->grp_count == port->max_grp) {
        MC_LOG(IROS_LOG_LEVEL_INF, "reach the max grp num!\n");
        return CS_E_RESOURCE;
    }    

    vid = record->vlanid;
    if(!mc_node->grp_tbl[vid]) {
        mc_node->grp_tbl[vid] = mc_grp_table_create();
        if(NULL == mc_node->grp_tbl[vid]) {
            MC_LOG(IROS_LOG_LEVEL_CRI, "creat grp table fail!\n");
            return CS_E_RESOURCE;
        }

        MC_LOG(IROS_LOG_LEVEL_INF, "creat grp table %d!\n", vid);
    }
    
    grp_table = mc_node->grp_tbl[vid];

    grp_state = mc_grp_create();
    if(NULL == grp_state) {
        MC_LOG(IROS_LOG_LEVEL_CRI, "creat grp entry fail!\n");
        return CS_E_RESOURCE;
    }

    MC_NEW_GRP_DUMP(record);

    /* init grp */
    grp_state->entry.mc_node = mc_node;
    memcpy(&grp_state->entry.mc_grp, &record->mc_grp, sizeof(ip_type_t));
    grp_state->entry.vlanid = record->vlanid;

    ret = mc_add_port_grp_entry(grp_state, port, record);
    if(CS_E_OK == ret) {
        mc_grp_add(grp_table, grp_state);
    }
    else {
        mc_grp_destroy(grp_state);
    }

    return ret;
}

cs_status mc_grp_record_handle(
    mc_node_t *mc_node, 
    mc_port_t  *port, 
    mc_group_record_list_t *record)
{        
    cs_status ret = CS_E_OK;
    cs_uint16 vid = 0;

    mc_group_node_t *grp_state = NULL;
    
    if(MC_IVL == mc_node->vlan_lrn_mode) {
        vid = record->entry.vlanid;
    }
    else {
        record->entry.vlanid = 0;
    }

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_grp_record_handle] port = %d, vid = %d, %s\n", 
        port->portid, vid, record->entry.flag ? "static" : "dynamic");

    if(!record->entry.flag && MC_MANUAL == mc_node->mode) { /* receive report pkt at manual mode */
        mc_port_group_state_list_t *port_grp  = 
            mc_port_grp_find_globally(mc_node, port, &record->entry.mc_grp);
        if(port_grp) {
            ret = mc_update_port_grp_entry(port_grp, record);
        }
    }
    else {  /* receive report at snooping mode or add mc ctrl entry manually */
        grp_state = mc_grp_find(mc_node, vid, &record->entry.mc_grp);
        if(grp_state) {
            mc_port_group_state_list_t *port_grp_state  = mc_port_grp_find(port, grp_state);
            if(port_grp_state) {    /* update port grp */                        
                ret = mc_update_port_grp_entry(port_grp_state, record);
            }
            else {  /* add new port to grp */
                if(record->entry.flag || MC_MANUAL != mc_node->mode) {
                    ret = mc_add_port_grp_entry(grp_state, port, &record->entry);
                }
            }
        }
        else {  /* new grp and create port grp */
            if(record->entry.flag || MC_MANUAL != mc_node->mode) {
                ret = mc_new_port_grp_entry(mc_node, port, &record->entry);
            }
        }
    }

    return ret;
}

cs_status  mc_grp_record_list_proc(mc_node_t *node, mc_port_t * port, cs_list * record_list)
{
    mc_group_record_list_t *record = NULL;    
    
    cs_status ret = CS_E_OK;
     
    cs_lst_scan(record_list , record , mc_group_record_list_t *) {
        ret = mc_grp_record_handle(node, port, record);
        if (ret) {
            return ret;            
        }
    }

   return ret;
   
 }

#ifdef HAVE_MC_REPORT_V3_TO_V2
static cs_uint8 g_mc_v3_to_v2_flag = 1;
cs_uint8 mcV3ToV2IsEnable()
{
	return g_mc_v3_to_v2_flag;
}

cs_uint8 mcV3ToV2Enable(cs_uint8 en)
{
	g_mc_v3_to_v2_flag = en;
	return g_mc_v3_to_v2_flag;
}

cs_status mc_grp_report_vchange_handler(mc_node_t * node, mc_object_t *msg)
{

	cs_status ret = CS_E_ERROR;
	cs_uint16 len_report;
	
	len_report = msg->len;
	#ifdef MC_VCHANGE
	int n = 0;
	cs_printf("rcv report msg:\n");
	for(n= 0 ; n < len_report; n++)
	{
		if(n%10 == 0)
			{
				cs_printf("0x%02x ",msg->frame[n]);
				cs_printf("\n");
			}
		else
			{
				cs_printf("0x%02x ",msg->frame[n]);
			}
	}
	cs_printf("\n");
	#endif
	if((msg->ip_type == MC_IPV4) && (msg->pdu_type == IGMP_V3_REPORT) && mcV3ToV2IsEnable())
	{
		mc_pdu_t *pdu = NULL;
		cs_uint32 dip = 0, i, groups = 0, length = 0;
		ip_type_t ip;
		mc_group_record_pdu_t * record = NULL;

		igmp_ip_header_t *ipv4_hdr = (igmp_ip_header_t*)(msg->frame+msg->ether_head_len);
		mc_ether_header_t * ether_hdr = (mc_ether_header_t*)msg->frame;
			pdu = (mc_pdu_t*)(msg->frame+msg->ether_head_len+msg->ip_head_len);
			#if 1
			record = (mc_group_record_pdu_t*)(((cs_int8*)pdu)+sizeof(mc_pdu_t));
			#else
			record = (mc_group_record_pdu_t*)(msg->fram + 30);
			#endif
			groups = ntohs(pdu->grp_key.reserved.num_grp_records);
			length = msg->len;
			#ifdef MC_VCHANGE
			diag_printf("record:0x%32x\n",record->mc_grp.ipv4);
			#endif
			for(i=0; i<groups; i++){

				if( (cs_uint32)record >= (cs_uint32)(msg->frame + length))
					break;

			if((record->record_type == MODE_IS_INCLUDE
					|| record->record_type == CHANGE_TO_INCLUDE_MODE) &&
					record->num_sources == 0)
				pdu->type = IGMP_V2_LEAVE;
			else
				pdu->type = IGMP_V2_REPORT;

			pdu->time_key.max_resp_time = 0;
			pdu->grp_key.ipv4_grp = record->mc_grp.ipv4;
			pdu->checksum = 0;

			pdu->checksum = inet_chksum(pdu, sizeof(mc_pdu_t));

		//	modified dest mac and ip address
		#ifdef MC_VCHANGE
			cs_printf("type:0x%02x\n",pdu->type);
		#endif
			dip = (pdu->type == IGMP_V2_REPORT)?record->mc_grp.ipv4:htonl(0xe0000002);

			ip.addr.ipv4 = ntohl(dip);
			ip.type = MC_IPV4;
			mc_ip_to_mac(&ip, ether_hdr->dest);

			ipv4_hdr->dst_ip = dip;
			ipv4_hdr->check = 0;
			ipv4_hdr->check = inet_chksum(ipv4_hdr, msg->ip_head_len);
			#if 0
			for(j = 0; j < 6; j++)
				{
					cs_printf("ether_hdr->dest: 0x%02x\n",ether_hdr->dest[j]);
				}
			#endif
			msg->len = msg->ether_head_len+msg->ip_head_len+sizeof(mc_pdu_t);

			MC_LOG(IROS_LOG_LEVEL_INF, "igmp mode: %d, up_port %p\r\n", node->mode, node->up_port);
			if(node->mode != MC_PROXY && node->up_port)
			{
				MC_LOG(IROS_LOG_LEVEL_INF, "mc up port id %d\r\n", node->up_port->portid);
				#ifdef MC_VCHANGE
				cs_printf("send report msg:\n");
				for(n = 0 ; n < len_report; n++)
					{
						if(n%10 == 0)
							{
								cs_printf("0x%02x ",msg->frame[n]);
								cs_printf("\n");
							}
						else
							{
								cs_printf("0x%02x ",msg->frame[n]);
							}
					}
				cs_printf("\n");
				#endif
				mc_fwd_msg(node->up_port, pdu->type, msg->frame, msg->len);
			}

			record = (mc_group_record_pdu_t*)(((cs_int8*)record)+sizeof(mc_group_record_pdu_t)+record->aux_data_len+record->num_sources*sizeof(cs_int32));
		}
		ret = CS_E_OK;
	}

	return ret;
}
#endif

void mc_report_proc(mc_node_t *node, mc_object_t *msg)
{   
    mc_port_t *port = msg->port;    

    if(mc_is_uplink_port(node, port->portid)) {
        return ;
    }

    if(mc_grp_record_parse(node,msg) == CS_E_OK) {
        /* record handle */
        if(CS_E_OK == mc_grp_record_list_proc(node, port, &msg->record_list)) {
			#ifdef HAVE_MC_REPORT_V3_TO_V2
			if((MC_PROXY != node->mode)&&(CS_E_OK != mc_grp_report_vchange_handler(node, msg))) {
                if(node->up_port) {
                    mc_fwd_msg(node->up_port, msg->pdu_type, msg->frame, msg->len);
                }
            }
			#else
            if(MC_PROXY != node->mode) {
                if(node->up_port) {
                    mc_fwd_msg(node->up_port, msg->pdu_type, msg->frame, msg->len);
                }
            }
			#endif
        }
    }
    
    mc_record_list_clear(&msg->record_list);
}

