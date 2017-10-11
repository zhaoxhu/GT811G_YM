function $() { 
  var elements = new Array(); 
  
  for (var i = 0; i < arguments.length; i++) { 
    var element = arguments[i]; 
    if (typeof element == 'string') 
      element = document.getElementById(element); 

    if (arguments.length == 1) 
      return element; 
      
    elements.push(element); 
  } 
  
  return elements; 
} 

function XMLHttp() 
{
	var _xmlhttp = null;
	var _sendMethod = "GET";
	var _handler = null;
	this.create = create;
	this.sendRequest = sendRequest;
	this.sendRequestSync = sendRequestSync;
	this.setSendMethod = setSendMethod;
	
	this.getResponseHeader  =  function(headerName)
	{
		return _xmlhttp.getResponseHeader(headerName);
	}
	create();
	
	function create() 
	{
		var aVersions = [ "MSXML2.XMLHttp.5.0",
		"MSXML2.XMLHttp.4.0","MSXML2.XMLHttp.3.0",
		"MSXML2.XMLHttp","Microsoft.XMLHttp"];
		
		try 
		{
			for (var i = 0; i < aVersions.length; i++) 
			{
				try
				{
					_xmlhttp = new ActiveXObject(aVersions[i]);
					if (null != _xmlhttp)
					{
						break;
					}
				}
				catch(e){
					_xmlhttp = null;
				}
			}
		}
		catch (e) 
		{
			try 
			{
				_xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
			}
			catch (oc) 
			{
				_xmlhttp = null;
			}
		}
		if ((_xmlhttp == null) && ((typeof XMLHttpRequest) != "undefined" )) 
		{
			_xmlhttp = new XMLHttpRequest();
		}
	}
	
	function setSendMethod(method) 
	{
		_sendMethod = method;
	}
	
	function sendRequest(url, body, fn) 
	{
		if (_xmlhttp == null) return null;
		_handler = fn;
		_xmlhttp.open(_sendMethod, url, true);
		_xmlhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		_xmlhttp.onreadystatechange = requestComplete;
		_xmlhttp.send(body);
	}
	
	function sendRequestSync(url, body, fn) 
	{
		if (_xmlhttp == null) return null;
		_handler = fn;
		_xmlhttp.open(_sendMethod, url, false);
		_xmlhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		_xmlhttp.onreadystatechange = requestComplete;
		_xmlhttp.send(body);
	}
	
	function requestComplete(responseText)
	{
		if (_xmlhttp.readyState == 4) 
		{
			_handler(_xmlhttp.responseText, _xmlhttp.status);
		}
	}
}

function evalJSON(json) 
{
	var obj = null;
	try 
	{
		obj = eval("(" + json + ")");
	} catch(E) 
	{
	}
	return obj ;
}

function displayObj(obj, show)
{

    if(show)
    {
        obj.style.visibility = "visible";
    }else{
        obj.style.display = "none";
    }
}


function clone(obj){
    if(obj == null || typeof(obj) != 'object') return obj;

    if(obj.constructor == Array) {
        var temp = [];
        for(var i = 0; i < obj.length; i++) {
            if(typeof(obj[i]) == 'object')   temp.push(clone(obj[i]));
            else temp.push(obj[i]);
        }
        return temp;
    }

    var temp = {};
    for(var key in obj) 
        temp[key] = clone(obj[key]);
    
    return temp;
}


function replaceString(str, template, replaceText) 
{
    var s = "";
    var s_s = '<!-- ' + template + ' BEGIN-->';
    var s_e = '<!-- ' + template + ' END-->';
    var start 	= str.indexOf(s_s);
    var end 	= str.indexOf(s_e);
    if (start == -1 || end == -1) return str;
    end += s_e.length;
    s += str.substring(0, start);
    s += replaceText;
    s += str.substr(end);
	return s;
}

function getTemplateString(str, template) 
{
	var s = '<!-- ' + template + ' BEGIN-->';
	var start 	= str.indexOf(s);
	var end 	= str.indexOf('<!-- ' + template + ' END-->');
	if (start == -1 || end == -1) return "";
	start += s.length;
	return str.substring(start, end);
}

function change_language(obj)
{
	for (var attr in obj)
	{
		try
		{
			switch (attr)
			{
				case "title":
					document.title = obj[attr];
					break;
				case "innerHTML":
					var o_arr = obj[attr];
					for (var k in o_arr)
					{
						try
						{
							$(k).innerHTML = o_arr[k];
						}
						catch(E)
						{
						}
					}
					break;
				case "value":
					var o_arr = obj[attr];
					for (var k in o_arr)
					{
						try
						{
							$(k).value = o_arr[k];
						}
						catch(E)
						{
					}
					}
					break;
				case "option_title":
					var o_arr = obj[attr];
					for (var k in o_arr)
					{
						try
						{
							for (var i=0; i<o_arr[k].length; i++)
							{
								try
								{
									$(k).options[i].title = o_arr[k][i];
								}
								catch(E)
								{
								}
							}
						}
						catch(E)
						{
						}
					}
					break;
				case "option_text":
					var o_arr = obj[attr];
					for (var k in o_arr)
					{
						try
						{
							for (var i=0; i<o_arr[k].length; i++)
							{
								try
								{
									$(k).options[i].text = o_arr[k][i];
								}
								catch(E)
								{
								}
							}
						}
						catch(E)
						{
						}
					}
				break;
				default:
			}
			}
		catch(E)
		{
		}
	}
}

function check_web_status( web_status )
{
    if(web_status == 200)
	    return 0;

    return -1;
}

function auto_detect_system_language()
{
	var sys_lan = false;
	try
	{
		sys_lan = navigator.browserLanguage.toLowerCase() == "zh-cn";
	}
	catch(e)
	{
		try
		{
			sys_lan = navigator.language.toLowerCase() == "zh-cn";
		}catch(e){}
	}
	return sys_lan;
}

String.prototype.Trim=function(){return this.replace(/(^\s*)|(\s*$)/g,   "");}

function cgi_header(  )
{
	var str = "";
	str += "?rand=" + Math.random();
    return str;
}

