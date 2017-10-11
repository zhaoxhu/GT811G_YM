// Local Lib here.
function open_close_menu(obj, open)
{
    if( obj.style.display == 'none' )
    {
        obj.style.display = ''
    }
    else{
        obj.style.display = 'none'
    }
}

function addEvent(obj, evt, fn)
{
    try { obj.addEventListener(evt, fn, false); return true;} catch(E) {}
	try { obj.attachEvent("on"+evt, fn); return true;} catch(E) {}
	return false;
}
function getEventElement(e)
{
	if (e.srcElement)
	{
		return e.srcElement;
	}
	else if (e.originalTarget)
	{
		return e.originalTarget;
	}
	return null;
}

var menu_list = [];
function addMenuEvent(name_src, name_dst) 
{
	try 
	{
		var obj_src = $(name_src);
		var obj_dst = $(name_dst);
		obj_dst.setAttribute("show", "false");
		obj_src.setAttribute("obj_name", name_dst);
		open_close_menu(obj_dst, false);
		addEvent(obj_src, "click", showHideMeun);
		menu_list.push(obj_src);
	} catch (e) 
	{
		alert(e);
	}
}
function showHideMeun(e) 
{
	var obj_src = getEventElement(e);
	try 
	{
		var obj_dst = $(obj_src.getAttribute("obj_name"));
		var str_show = "collapse";
		if (obj_dst.getAttribute("show") == "false") 
		{
			obj_dst.setAttribute("show", "true");
			open_close_menu(obj_dst, true);
		} else 
		{
			open_close_menu(obj_dst, false);
			obj_dst.setAttribute("show", "false");
		}
	} catch(e) 
	{
		alert(e);
	}
}
