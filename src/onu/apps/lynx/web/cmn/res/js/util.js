
function isNull(str) 
{
	if (str == "") return true;
	var regu = "^[ ]+$";
	var re = new RegExp(regu);
	return re.test(str);
}


function verifyIP(ip, mask) 
{
	var ip_pattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
	var ip_array = ip.match(ip_pattern);
	var mask_array = mask.match(ip_pattern);
	
	if (ip_array == null || mask_array == null)
		return false;
	else
	{
		var arr_ip = ip.split(".");
		var arr_mask = mask.split(".");
		for (var i=0; i<4; i++) 
		{
			if (arr_ip[i] > 255 || arr_mask[i] > 255) 
			{
				return false;
			}
		}
		
		if (arr_ip[0] >= 224) 
		{
			return false;
		}
		if(arr_mask[0] == 255 && arr_mask[1] == 255 && arr_mask[2] == 255 && arr_mask[3] == 255)
		{
			return false;
		}
		if(arr_ip[0] == 0)
		{
			return false;
		}
		if(arr_ip[0] == 127)
		{
			return false;
		}
		var subnet=0;
		for(var j=0;j<4;j++){
			subnet=subnet*256;
			subnet+=(arr_ip[j] & (~arr_mask[j]));
		}
		
		if(subnet==0) return false;
		var subnet_bc=0;
		for(var k=0;k<4;k++){
			subnet_bc=subnet_bc*256;
			subnet_bc+=(arr_ip[k] | arr_mask[k]);
		}
		if(subnet_bc==0xffffffff) return false;
	}
	return true;
}

function verifySubNet(ip, mask) 
{
	var ip_pattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
	var ip_array = ip.match(ip_pattern);
	var mask_array = mask.match(ip_pattern);
	
	if (ip_array == null || mask_array == null)
		return false;
	else
	{
		var arr_ip = ip.split(".");
		var arr_mask = mask.split(".");
		for (var i=0; i<4; i++) 
		{
			if (arr_ip[i] > 255 || arr_mask[i] > 255) 
			{
				return false;
			}
		}
		
		if (arr_ip[0] >= 224) 
		{
			return false;
		}
		if(arr_mask[0] == 255 && arr_mask[1] == 255 && arr_mask[2] == 255 && arr_mask[3] == 255)
		{
			return false;
		}
		if(arr_ip[0] == 0)
		{
			return false;
		}
		if(arr_ip[0] == 127)
		{
			return false;
		}

		var subnet_bc=0;
		for(var k=0;k<4;k++){
			subnet_bc=subnet_bc*256;
			subnet_bc+=(arr_ip[k] | arr_mask[k]);
		}
		if(subnet_bc==0xffffffff) return false;
	}
	return true;
}

function verifyMask(mask) 
{
	var ip_pattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
	var mask_array = mask.match(ip_pattern);
	if (mask_array == null)
		return false;
	else
	{
		var arr = mask.split(".");
		if(arr[0] == 0 )
			return false;
		
		for (var i=0; i<4; i++) 
		{
			if (arr[i] > 255) 
			{
				return false;
			}
			
			if(arr[i] < 255)
			{
				for(var j = i+1; j <4 ; j++){
					if(arr[j] != 0)
						return false;
				}
				if(arr[i] != 0 ){
					/*Check last value */
					var oneCount = 0;
					var item4 = (~arr[i] + 1)&0xff;
					/*make sure the item4 only has one bit is 1*/
					for (var k = 8; k >= 0; k--){

						if(((item4)&(0x1 << k)) != 0)
							oneCount ++;
					}
					
					if(oneCount > 1)
						return false;
				}
			}
			
		}
		if(arr[0] == 255 && arr[1] == 255 && arr[2] == 255 && arr[3] == 255)
		{
			return false;
		}

		/* 128.0.0.0 is the max mask */
        /* 255.255.255.252 is the min mask */
		if((arr[0] != 255 && arr[0] < 0x80) || arr[3] > 252)
			return false;		
		
	}
	return true;
}

function isValidNum(str) {
	str = str.replace(/(^[\s]*)|([\s]*$)/g, "");
	if( str !="" && !isNaN( str ) ){
		return 0;
	}else if( str !="" && isNaN( str ) ){
		return 1;
	}else {
		return 2;
	}
}
function checkChar(num) 
{
	var s = "0123456789ABCDEF";
	for (var i=0; i<s.length; i++) 
	{
		if (s.charCodeAt(i) == num) 
		{
			return true;
		}
	}
	return false;
}
function verifyMac(mac) 
{
	if (mac.length != 17) 
	{
		return false;
	}
	var mac_arr =  mac.toUpperCase().split(":");
	if (mac_arr.length != 6) 
	{
		return false;
	}
	
	/*all Zero */
	var i = 0;
	for ( i=0; i<mac_arr.length; i++) {
			if(mac_arr[i] != 0)
				break;	
	}
	
	if(i == mac_arr.length)
			return false;
	/*All ff*/
	for ( i=0; i<mac_arr.length; i++) {
			if(mac_arr[i].charCodeAt(0) != 'f' ||
				mac_arr[i].charCodeAt(1) != 'f')
				break;	
	}
	
	if(i == mac_arr.length)
			return false;
	
	for ( i=0; i<mac_arr.length; i++) 
	{
		if (!checkChar(mac_arr[i].charCodeAt(0)) 
		|| !checkChar(mac_arr[i].charCodeAt(1))) 
		{
			return false;
		}
	}
	 
	/*Mac 0 can not start */
	if((mac_arr[0]&0x01) != 0){
		return false;
	} 
	 
	return true;
} 


function macStrToChar(s) {
	var mArray = s.split(":");
	return mArray;
}


function isKeyAndLengthOK(s,len)
{
	if (s.length!=len)
		return false;
	var mArray = s.split(":");
	if (mArray.length != 6)
		return false;
	for(i=0;i<6;i++)		
	{
		s1 = mArray[i];
		for(j=0;j<2;j++)
		{
			c=s1.charAt(j);
			if((c!='0')&&(c!='1')&&(c!='2')&&(c!='3')&&(c!='4')&&(c!='5')&&(c!='6')&&(c!='7')&&(c!='8')&&(c!='9')&&(c!='a')&&(c!='b')&&(c!='c')&&(c!='d')&&(c!='e')&&(c!='f')&&(c!='A')&&(c!='B')&&(c!='C')&&(c!='D')&&(c!='E')&&(c!='F'))
			return false;
		}
	}
	return true;
}

function isBrodcastMac(mac)
{
	for(i=0;i<mac.length;i++)		
	{
		c = mac.charAt(i);
		if(c != ':')
		{
			if((c != 'f') && (c != 'F') )
			{
				return false;
			}
		}
	}
	return true;
}

function isMulticastMac(mac)
{
	c = mac.charAt(1);
	str = "0x" + c;
	nVar = parseInt(str , 16);
	if((nVar % 2) > 0)
	{
		return true;
	}
	return false;
}

