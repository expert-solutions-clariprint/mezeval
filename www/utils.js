
function switchTreeView(event,tr)
{
	var i;
	var newmode = null;
	var refdeep = parseInt(tr.getAttribute("treeviewdeep"));
	
	var nextnode = tr.nextSibling;
	while (nextnode != null)
	{
		if (nextnode.nodeName == "TR")
		{
			var deep = parseInt(nextnode.getAttribute("treeviewdeep"));
			if (deep > refdeep)
			{
				if (newmode == null) {
					if (nextnode.style.display == 'none') {
						newmode = tr.style.display;
						$(tr).addClassName('openfolder');
					}
					else {
						newmode = 'none';
						$(tr).removeClassName('openfolder');
					}
				}
				if (newmode == 'none')
				{
					nextnode.style.display = newmode;
					if (!$(nextnode).hasClassName('nofolder'))
						$(nextnode).removeClassName('openfolder');
				}
				else if (refdeep == (deep - 1) || (event.shiftKey &&  deep > refdeep))
				{
					nextnode.style.display = newmode;
					if (!$(nextnode).hasClassName('nofolder') && event.shiftKey)
						$(nextnode).addClassName('openfolder');
				}
			} else break;
		}
		nextnode = nextnode.nextSibling;
	}
	alternateTable(tr.parentNode.parentNode);
}
function alternateTableEvent(evet)
{
	alternateTable(this);
}

function alternateTable(t)
{
	var tbi = 0;
	for(tbi = 0; tbi < t.tBodies.length; tbi++)
	{
		alternateTableBody(t.tBodies[tbi]);
	}
	t.observe("xl:updatecontent",alternateTableEvent);
}
function alternateTableBody(tbody)
{
	var ri = 0;
	var pair = false;
	for(ri = 0; ri < tbody.rows.length; ri++)
	{
		var row = $(tbody.rows[ri]);
		row.removeClassName("alternate");
		if (row.visible())
		{
			if (pair) row.addClassName("alternate");
			pair = !pair;
		}
	}
}

function alternateTables(evt)
{
	var tables = Element.select(Event.element(evt),"table.alternate"); // $$("table.alternate");
	tables.each(alternateTable);
}

$(document).observe("dom:loaded", alternateTables);

function trimString(str)
{
	return str.replace(/^\s+|\s+$/g,'');
	return str;
}

function addCssClass(obj,class_name)
{
	obj.className = trimString(obj.className + " " +  class_name);
/*	obj.class = trimString(obj.class + " " +  class_name); */
}

function rmCssClass(obj,class_name)
{
	if (obj.className != null)
		obj.className = trimString(obj.className.replace(class_name,""));
}


function checkAdbloc()
{
    if (document.baseURI.match("local.mezeval.fr")) return;
	if (typeof google_ad_client != "undefined")
	{
		if (!window.google_new_domain_enabled)
		{
			alert("Attention : vous utilisez un bloqueur de publicité. Hors la publicité est notre moyen de subsistance vous permettant de benéficier d'un outil libre d'accès. Veuiller contacter nos services si vous désirez une version sans annonces. Merci de votre compréhention.");
		}
	}
}

$(document).observe("dom:loaded", checkAdbloc);


function searchOption(input_id)
{
	var key = $(input_id).value;
	if (key != "")
	{
		var rgx = new RegExp(key,"i");
		var select = $($(input_id).getAttribute("target"));
		if (select)
		{
			var lastsearchkey = "";
			var lastsearchindex = 0;
			if (select.readAttribute('lastsearchkey') != null)
			{
				lastsearchkey = select.readAttribute('lastsearchkey');
			}
			if (key != lastsearchkey) {
				select.writeAttribute('lastsearchkey',key);
				select.writeAttribute('lastsearchindex',lastsearchindex);
			} else {
				if (select.readAttribute('lastsearchindex'))
					lastsearchindex = select.readAttribute('lastsearchindex');
			}
			var i = lastsearchindex;
			var loop = false;
			while (true)
			{
				if ( loop && i == lastsearchindex)
				{
					break;
				} else if (i >= select.options.length)
				{
					i = 0;
					loop = true;
				} else if (select.options[i].innerHTML.match(rgx))
				{	
					select.options[i].selected = true;
					lastsearchindex = i + 1;
					select.selectedIndex = i;
					break;
				} else { i++ };
			}
			select.writeAttribute('lastsearchindex',lastsearchindex);
		}
	}
}

function searchTable(input_id)
{
	if (input = $(input_id))
	{
		if (table = $(input.readAttribute("target")))
		{
			if (cols = input.readAttribute("target_cols"))
			{
				var bi;
				var regex = null;
				if (input.value != "")
				{
					regex = new RegExp(input.value,"i");
				}
				
				var col_ids = $w(cols);
				
				for(bi= 0; bi < table.tBodies.length ; bi++)
				{
					tbody= table.tBodies[bi];
					var ri = 0;
					for (ri = 0; ri < tbody.rows.length; ri++)
					{
						row = $(tbody.rows[ri]);
						if (regex == null)
							row.show();
						else {
							hide = true;
							var colid =0
							for(colid =0 ; colid < col_ids.length;colid++  )
							{
								if (row.cells[col_ids[colid]].innerHTML.match(regex)) hide = false;
							}
							if (hide) row.hide();
							else row.show();
						}
					}
				}
			} else alert("cols undifined");
		}
	}
	alternateTables();
}
function searchBoxEventHandler(event)
{
	if (event.keyCode == 13)
	{
		Event.stop(event);
		var element = Event.element(event);
		searchTable(element);
		return false;
	}
}
function initSearchBox(box)
{
	$(box).observe("keydown", searchBoxEventHandler);
	
}
function initSearchBoxes()
{	
	$$("input.searchbox").each(initSearchBox);
}

$(document).observe("dom:loaded", initSearchBoxes);




		var TableKeyboardNavigator = Class.create(
			{
				mode:null,
				initialize: function(table)
				{
					var inputs = table.select('input');
					for(var i = 0;i<inputs.length;i++ )
					{
						this.attachEvents(inputs[i]);
					}
				},
				attachEvents: function(elem)
				{
					elem.observe('keyup',this.onkey.bind(this));
				},
				nextRow: function(elem,keyCode)
				{
					if (keyCode == Event.KEY_DOWN)
						return elem.next();
					else return elem.previous();
				},
				onkey: function(event)
				{
					var elem = Event.element(event);
					if (event.keyCode == Event.KEY_DOWN || event.keyCode == Event.KEY_UP )
					{
						var td = elem.firstAncestor("TD");
						if (td)
						{
							var row = td.parentNode;
							var idx = row.childElements().indexOf(td);
							row = this.nextRow(row,event.keyCode);
							while (row)
							{
								var subs = row.childElements();
								if (subs.length > idx)
								{
									var inputs = subs[idx].select("input");
									if (inputs.length>0)
									{
										row.show();
										inputs.first().focus();
										break;
									}
								}
								row = this.nextRow(row,event.keyCode);
							}
						}
					}
				}
			});
		TableKeyboardNavigator.setup = function(evt)
		{
			console.log("TableKeyboardNavigator.setup");
			Element.select(Event.element(evt),"table.keyboardNavigate").each(function(t){new TableKeyboardNavigator(t);});
		}
		document.observe("dom:loaded",TableKeyboardNavigator.setup);
