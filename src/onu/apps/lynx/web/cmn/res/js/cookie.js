
function getCookie(sName)
{
    var aCookie = document.cookie.split(";");
    for (var i=0; i < aCookie.length; i++)
    {
        var aCrumb = aCookie[i].split("=");
        if (sName == aCrumb[0])
        {
            return aCrumb[1];
        }
    }
    return null;
}

function LoginUser(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[0];
    }
    catch(E){}
    return "";
}

function getAdminName(sName)
{
	try
	{
		var login_user = getCookie(sName).split("|");
		return login_user[1];
	}
	catch(E){}
	return "";
}
function getUserName(sName)
{
	try
	{
		var login_user = getCookie(sName).split("|");
		return login_user[2];
	}
	catch(E){}
	return "";
}

function getAdminLevel(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[1];
    }
    catch(E){}
    return "";
}
function getUserLevel(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[2];
    }
    catch(E){}
    return "";
}

function getGuestLevel(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[3];
    }
    catch(E){}
    return "";
}

function getCurrentLevel(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[4];
    }
    catch(E){}
    return "";
}


function getWebLanguage(sName)
{
	try
	{
		var login_user = getCookie(sName).split("|");
		return login_user[3];
	}
	catch(E){}
	return "";
}


function getWebUserMagtLanguage(sName)
{
    try
    {
        var login_user = getCookie(sName).split("|");
        return login_user[5];
    }
    catch(E){}
    return "";
}

function getCookieVal (offset)
{
    var endstr = document.cookie.indexOf (";", offset);
    if (endstr == -1)
        endstr = document.cookie.length;
    return unescape(document.cookie.substring(offset, endstr));
}
function GetCookie (name)
{
    var arg = name + "=";
    var alen = arg.length;
    var clen = document.cookie.length;
    var i = 0;
    while (i < clen)
    {
        var j = i + alen;
        if (document.cookie.substring(i, j) == arg)
            return getCookieVal (j);
        i = document.cookie.indexOf(" ", i) + 1;
        if (i == 0)
            break;
    }
    return ;
}
function SetLangCookie(value)
{
    var CookieName = "user";
    var cval = GetCookie (CookieName);
    if (cval == null)
        return;
    var cookie = cval.split("|");
    var tempcookie = CookieName +"=", tempval = "";
    var i = 0;
    for(i = 0; i< cookie.length; i++)
    {
        if (i)
            tempval += "|";
        if (i == 3)
            tempval += value;
        else
            tempval += cookie[i];
    }
    tempcookie +=tempval;
    document.cookie= tempcookie;
}


