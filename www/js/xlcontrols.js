/************************************************************
 * XL Controls
 * Web 2.0 Tools by EXPERT SOLUTIONS
 * TOUT DROITS RESERVEES
 */

/************************************************************
 * Check dependencies
 */
if(typeof Prototype == 'undefined')
  throw("controls.js requires including script.aculo.us' effects.js library");


/************************************************************
 * NameSpace
 */

if(typeof XL == "undefined")
{
	XL = {
	version : "1.0.0"
		};
}

/************************************************************
 * Debug / IE hook
 */
if (typeof console == 'undefined')
{
	console = {
		log: function() {}
	}
}


/************************************************************
 * DatePicker
 * Usefull date picker widget
 */
XL.DatePickerCurrentWidget=null;
XL.DatePicker = Class.create({
	target:null,
	updateHandler:null,
	domelement:null,
	day: 1,
	month: 1,
	year: 2009,
	weeknum: 1,
	
	isinput: true,

	mode:'fr',
	firstday : 1,
	
	selectedDate: function()
		{
			if (this.isinput)
				return Date.parserUserDate(this.target.value);
			else return Date.parserUserDate(this.target.innerHTML);
		},

	firstDayOfWeek: function(d)
		{
			
			return  new Date(d.getFullYear(),
							d.getMonth(),
							d.getDate() + (this.firstday - d.getDay()));
		},
	day_names : {0:"Lu",1:"Ma",2:"Me",3:"Je",4:"Ve",5:"Sa",6:"Di"},
	month_names : {0:"Janvier",1:"Février",2:"Mars",
					3:"Avril",4:"Mai",5:"Juin",6:"Juillet",
					7:"Août",8:"Septembre",9:"Octobre",
					10:"Novembre",11:"Decembre"},

	toString: function()
		{
			return "[DatePicker]";
		},
	initialize: function(elem,handler)
		{
			this.updateHandler=handler;
			this.target = elem;
			if (elem.tagName != "INPUT")
				this.isinput = false;
			elem.observe('click',this.present.bind(this));
			if (this.isinput)
				elem.observe('blur',this.onBlur.bind(this));
			
			var now = this.selectedDate();
			this.year = now.getFullYear();
			this.month = now.getMonth() + 1;
			this.day = now.getDate();
			
		},
	
	onBlur: function(evt)
		{
			if (this.target.value != "")
			{
				var d = Date.parserUserDate(this.target.value);
				if (d)
				{
					this.target.value = d.format("dd/mm/yyyy");
				}
			}
			this.hide();
		},
	
	hide: function(evt)
		{
			if (this.domelement)
				this.domelement.hide();
			XL.DatePickerCurrentWidget = null;
			if (this.documentEventListener != null)
			{
				document.stopObserving('mousedown',this.documentEventListener);
				this.documentEventListener = null;
			}
		},
	
	present: function(event)
		{
			if (XL.DatePickerCurrentWidget != null)
				XL.DatePickerCurrentWidget.hide();
			if (this.documentEventListener == null)
				this.documentEventListener = this.hide.bindAsEventListener(this);
			Event.observe(document,'mousedown',this.documentEventListener);
//			this.element().absolutize();
			var pos = this.target.cumulativeOffset();
			this.element().setStyle({"top": (pos.top + this.target.getHeight() + 3) + "px","left":(pos.left + "px")});
			if (this.target.value != "")
			{
				var selected_date = this.selectedDate();
				this.year = selected_date.getFullYear();
				this.month = selected_date.getMonth() + 1;
				this.day = selected_date.getDate();
				this.update();
			}
			XL.DatePickerCurrentWidget = this;
			this.element().show();
		},
	previousYear: function(event)
		{
			this.year--;
			this.update();
			Event.stop(event);
		},
	nextYear: function(event)
		{
			this.year++;
			this.update();
			Event.stop(event);
		},
	previousMonth: function(event)
		{
			
			this.month--;
			if (this.month < 1) { this.month = 12; this.year --; }
			this.update();
			Event.stop(event);
		},
	nextMonth: function(event)
		{
			this.month++;
			if (this.month > 12) { this.month = 1; this.year ++; }
			this.update();
			Event.stop(event);
		},
		
	element: function()
		{
			if (this.domelement == null)
			{
				this.domelement = new Element("TABLE", {"class":"XLDatePicker"});
				this.domelement.observe('selectstart', function() {return false;});
				this.domelement.observe('drag', function() {return false;});
				var head = new Element("THEAD");				
				this.domelement.appendChild(head);
				
				/* années */
				var year_row = new Element("TR");
				var previous_year = new Element("TD", {"colspan":2}).update("&lt;");
				previous_year.observe('mousedown',this.previousYear.bindAsEventListener(this));
				year_row.appendChild(previous_year);
				this.td_year = new Element("TH", {"colspan":4});
				year_row.appendChild(this.td_year);
				var next_year = new Element("TD", {"colspan":1}).update("&gt;");
				next_year.observe('mousedown',this.nextYear.bindAsEventListener(this));
				year_row.appendChild(next_year);

				var close_year = new Element("TD", {"colspan":1}).update("x");
				close_year.observe('close_year',this.hide.bindAsEventListener(this));

				year_row.appendChild(close_year);
				head.appendChild(year_row);
				
				/* mois */
				var month_row = new Element("TR");
				var previous_month = new Element("TD", {"colspan":2}).update("&lt;");
				previous_month.observe('mousedown',this.previousMonth.bindAsEventListener(this));
				month_row.appendChild(previous_month);
				this.td_month = new Element("TH", {"colspan":4});
				month_row.appendChild(this.td_month);
				var next_month = new Element("TD", {"colspan":1}).update("&gt;");
				next_month.observe('mousedown',this.nextMonth.bindAsEventListener(this));
				month_row.appendChild(next_month);
				head.appendChild(month_row);

				var dayrow = new Element("TR");
				head.appendChild(dayrow);
				dayrow.appendChild(new Element("TD"));
				for(var j = 0; j < 7 ; j ++)
				{
					dayrow.appendChild(new Element("TH").update(this.day_names[j]));
				}
				dayrow.appendChild(new Element("TH").update("&nbsp&nbsp"));
				this.body = new Element("TBODY");
				this.domelement.appendChild(this.body);
				this.domelement.style.display = 'block';

				var tfoot = new Element("TFOOT");
				this.domelement.appendChild(tfoot);
				var trf = new Element("TR");
				tfoot.appendChild(trf);
				this.td_current_date = new Element("TD",{colspan:4});
				this.td_current_date.observe("mousedown",this.gotoSelected.bindAsEventListener(this));
				this.td_current_date.observe("dblclick",this.clickOnDay.bindAsEventListener(this));
				trf.appendChild(this.td_current_date);
				var td_today = new Element("TD",{colspan:5});
				td_today.date = new Date();
				td_today.insert(td_today.date.format("dd/mm/yyyy"));
				td_today.observe("mousedown",this.gotoSelected.bindAsEventListener(this));
				td_today.observe("dblclick",this.clickOnDay.bindAsEventListener(this));
				trf.appendChild(td_today);

				this.update();
				$('body').insert(this.domelement);
//				this.target.insert({after: this.domelement});
			}
			return this.domelement;
		},
	
	gotoSelected: function(evt)
		{
			var d = Event.element(evt).date;
			this.day = d.getDate();
			this.month = d.getMonth() + 1;
			this.year = d.getFullYear();
			this.update();
			Event.stop(evt);
		},
		
	clickOnDay: function(event)
		{
			if (this.target)
			{
				if (this.isinput)
					this.target.value = Event.element(event).date.format("dd/mm/yyyy");
				else 
					this.target.update(Event.element(event).date.format("dd/mm/yyyy"));
				this.element().hide();
			}
			if (this.updateHandler)
				this.updateHandler(this.target,Event.element(event).date.format("dd/mm/yyyy"));
		},
	update: function()
		{
			if (this.body == null) this.element();
			
			var selected_date = this.selectedDate();
			
			var selected_week = selected_date.getWeek(this.firstday);

			this.td_current_date.update(selected_date.format("dd/mm/yyyy"));
			this.td_current_date.date = selected_date;

			this.td_month.update(this.month_names[this.month - 1]);
			this.td_year.update(this.year);
			var first_month_day = new Date(this.year,this.month - 1 , 1);
			var last_month_day = new Date(this.year,this.month , 0);
			var current_day = this.firstDayOfWeek(first_month_day);
			this.body.update();
			while (current_day < last_month_day)
			{
				var row = new Element("TR");
				this.body.appendChild(row);
				var weeknum = current_day.getWeek(this.firstday);
				var selected_row = false;
				if ((weeknum == selected_week) && (selected_date.getFullYear() == this.year))
				{
					row.addClassName("selected");
					selected_row = true;
				} else console.log("weeknum  %s != selected_week %s ",weeknum,selected_week);
				row.appendChild(new Element("TH").update(weeknum));
				for(var j = 0; j < 7; j++)
				{
					var nd = current_day.getDate();
					var d = new Element("TD").update(nd);
					if (selected_row && nd == selected_date.getDate( ))
						d.addClassName("selected");
					row.appendChild(d);
					d.observe('mousedown',this.clickOnDay.bindAsEventListener(this));
					d.date = current_day;
					if (current_day.getMonth() + 1 != this.month)	d.addClassName('Off');
					current_day = new Date(current_day.getFullYear(),current_day.getMonth(),current_day.getDate() + 1);
				}
			}
		}
});

Event.observe(document,"dom:loaded",function(){ $$('.XLDatePicker').each(function(obj){ new XL.DatePicker(obj)})});

Event.observe(document,"dom:loaded",function() {
	var elem = Element.down(document,'.XLFirstFocus');
	if (elem != null) elem.focus();});


function printFrame(frame_name)
{
	var w = window;
	if (window.parent) w = window.parent;
	w.frames[frame_name].focus();
	w.frames[frame_name].print();
}


XL.ProgessBar = Class.create({
	
	initialize: function(msg)
		{
			var dd = new Element("DIV");
			dd.insert(msg);
			Element.down(document,"body").insert(dd);
			var x = new Control.Modal(dd,{
					overlayOpacity: 0.75, 
					fadeDuration: 0.2,
					className: 'modal',  
					fade: true});
			x.open();
		}
});
XL.FormProgessBar = Class.create({
	form:null,
	initialize: function(form)
		{
			this.form = $(form);
			this.form.observe('submit',this.submitHandler.bindAsEventListener(this));
		},
	submitHandler: function(evt)
		{
			new XL.ProgessBar("Envoi des données au serveur, veuillez patienter");
		}
});

Event.observe(document,"dom:loaded",function(){ $$('form.XLProgressBarOnSubmit').each(function(obj){ new XL.FormProgessBar(obj)})});

