#ifndef _MC_PARSER_H
#define _MC_PARSER_H

#include "mc_core.h"

extern cs_status mc_pkt_object_build(mc_node_t *node, cs_pkt_t *pkt, mc_object_t *message);
extern void mc_pkt_obj_update(cs_pkt_t *pkt, mc_object_t *msg);

extern cs_status igmp_query_parse(mc_node_t *node, mc_object_t *pObj);
extern cs_status mld_query_parse(mc_node_t *node, mc_object_t *pObj);
extern cs_status mc_query_parse(mc_node_t *node, mc_object_t *pObj);
extern cs_status mc_grp_record_parse(mc_node_t *node,mc_object_t *pObj);

#endif  /* #ifndef _MC_PARSER_H */

