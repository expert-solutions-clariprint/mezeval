

function displayDialog(url,width,height)
{
	var div = $("dialog_box");
	if (div == null) div = new Element('DIV',{ 'class': 'dialog' , id : 'dialog_box'});
	div.setAttribute('dialogBox','1');
	div.style.display = "block";
	div.style.left = "50%";
	div.style.top = "50%";
	div.style.position = "Fixed";
	div.style.width = width + "px";
/*	div.style.height = height + "px"; */
	div.style.marginLeft = (-width / 2).toString() + "px" ;
	div.style.marginTop = (-height / 2).toString() + "px" ;
	if (url != null)
	{
		document.body.appendChild(div);
		var req = new Ajax.Updater('dialog_box',url,{ method:'post' });
	}
	return div;
}

function dialogCancel(obj)
{
	if (obj.hasAttribute('dialogBox')) obj.remove();
	else if (obj.parentNode != null) dialogCancel(obj.parentNode);
}


function trace(str)
{
	var div = $("debuglog");
	if (div == null) 
	{
		div = new Element("DIV", {"id": "debuglog"});
		document.body.appendChild(div);
	}
	div.insert(str);
}


function swapImageSrc(img,tag)
{
	var src = img.getAttribute(tag);
	img.setAttribute(tag,img.src);
	img.src = src;
}


var CURRENT_POPUP = null;
function showProgessionEleve(src_obj,prog,eleve)
{
	var src = $(src_obj);
	if (CURRENT_POPUP != null) CURRENT_POPUP.close();
	if (src.mz_popupprogressions == null)
	{
		var url = "export.php?module=historique_progression&no_body_header=1&progression=" + prog;
		if (eleve)
		{
			url += "&eleve=" + eleve;
		}
		src.writeAttribute('href',url);
		src.mz_popupprogressions = new Control.Window(src,{
			className: 'simple_popup',
			position: 'center',
			width: 600,
			closeOnClick: 'container',
			draggable : false,
			offsetLeft: 150,
			afterOpen: function () {alternateTables();}
			}); 
		src.mz_popupprogressions.href = url;
	}
	src.mz_popupprogressions.open();
	CURRENT_POPUP = src.mz_popupprogressions;
	return false;
}


