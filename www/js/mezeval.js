
Mz = {};

Mz.log = function(txt)
	{
		if (window.console) console.log(txt);
	}

Mz.TableController = Class.create({
	
	
});

Mz.FormController = Class.create({
	initialize: function(form)
		{
			this.form = form;
			this.form.observe('submit',this.onSubmit.bindAsEventListener(this));
		},
	onSubmit: function(evt)
		{
			alert("submit");
			Event.stop(evt);
			return false;
		}
});
Mz.FormController.setup = function(){}
Event.observe(document,"dom:loaded",Mz.FormController.setup);

Mz.TableController = Class.create({
	onProgress:false,
	table: null,
	
	url: null,
	deleteMsg:'Attention, les données associées seront supprimées !',
	_progess: null,
	initialize: function(t)
		{
			this.table = t;
			if (t.hasAttribute('controlUrl'))
				this.url = t.readAttribute('controlUrl');
			this._update(); 
			var addb = t.down(".add_button");
			if (addb) addb.observe('click',this.addAction.bindAsEventListener(this));
			
		},
	progress: function()
		{
			if (!this._progress)
			{
				this._progress = new Element("TR",{"class":"progressBar"});
				var td = new Element("TD");
				td.insert(new Element("IMG",{'src':'/img/spin.gif'}));
				this._progress.insert(td);
			}
			return this._progress;
		},
	startProgress: function()
		{
			this.table.down("tbody").insert(this.progress());
			this.onProgress = true;
		},
	stopProgress: function()
		{
			if (this.onProgress)
				this.progress().remove();
			this.onProgress = false;
		},
	load : function()
		{
			this.startProgress();
		},
	_update: function()
		{
			this.table.down('tbody').update();
			this.startProgress();
			new Ajax.Request(this.url,{
				'method': "POST",
				'onComplete': this.updateHandler.bind(this),
				'parameters': {
					"action":"load"
				}
			});
		},
	updateObject: function(x)
		{
			Mz.log("updateObject("+ x + ")");
			new Ajax.Request(this.url,{
				'method':'POST',
				'parameters':
					{
						'action':'update',
						'obj': Object.toJSON(x)
					} /* ,
				'onSuccess': this.addHandler.bindAsEventListener(this) */
			});
		},
	updateHandler: function(resp)
		{
			this.stopProgress();
			var i =0;
			var b = this.table.down('tbody');
			for(i=0;i < resp.responseJSON.length; i++)
			{
				var row= this.row(resp.responseJSON[i]);
				b.insert(row);
			}
			b.fire("xl:updatecontent");
		},
	row: function(obj)
		{
			var tr = new Element("TR");
			var td = new Element("TD");
			tr.insert(td);
			td.insert("pp");
			return tr;
		},
	showObject: function(obj)
		{
			
		},
	addObject: function(x)
		{
			Mz.log("addObject("+ x + ")");
			new Ajax.Request(this.url,{
				'method':'POST',
				'parameters':
					{
						'action':'add',
						'obj': Object.toJSON(x)
					},
				'onSuccess': this.addHandler.bindAsEventListener(this)
			});
			
		},
	addHandler: function(resp)
		{
			this.table.down('tbody').update();
			this.updateHandler(resp);
		},
	addAction: function(evt)
		{
			alert("addAction");
		},
	deleteAction: function(evt)
		{
			var row=  Event.element(evt).up("TR");
			var obj = row.mzObj;
			Mz.log(this);
			if (confirm(this.deleteMsg + "(" + this.info(obj) + ")"))
			{
				Mz.log("delete");
				new Ajax.Request(this.url,{
					'method':'POST',
					'parameters':
						{
							'action':'delete',
							'id':obj.id
						},
					'onSuccess': this.deleteHandler.bindAsEventListener(this,row)
				});
			}
		},
	deleteHandler: function(resp,row)
		{
			Mz.log(resp);
			if (resp.responseJSON)
			{
				row.remove();
				this.table.fire("xl:updatecontent");
			}
		},
	info: function()
		{
			return "";
		}
});

Mz.ElevesTableController = Class.create(Mz.TableController,{
	url:"/ajax/eleves.php",
	info: function(obj)
	{
		return obj.nom + " " + obj.prenom;
	},
	
	addAction: function(evt)
		{
			var x = {};
			x.nom = this.table.down("input#eleve_add_nom").value;
			x.prenom = this.table.down("input#eleve_add_prenom").value;
			x.date_naissance =  this.table.down("input#eleve_add_date").value;
			this.addObject(x);
		},
	row: function(obj)
	{
		var tr = new Element("TR");
		var td = new Element("TD").insert(obj.nom);
		tr.insert(td);
		td = new Element("TD").insert(obj.prenom);
		tr.insert(td);
		td = new Element("TD").insert("");
		tr.insert(td);
		td = new Element('TD',{'class':'action'}).insert(
			new Element('A',{'href':'?module=eleve&eleve_id=' + obj.id}).insert("détail"));
		tr.insert(td);
		td = new Element('TD',{'class':'action'}).insert(new Element("IMG",{'src':'/img/delete.png'}));
		td.observe('click',this.deleteAction.bindAsEventListener(this));
		tr.insert(td);
		tr.mzObj = obj;
		return tr;
	}
});

Event.observe(document,'dom:loaded',function(){
	$$("table.ElevesTableController").each(
		function(x){ new Mz.ElevesTableController(x)});});

Mz.InPlaceEditors = new Array();
Mz.InPlaceEditor = Class.create({
	handler:null,
	onEdit:false,
	value:null,
	element:null,
	input:null,
	div:null,
	initialize: function(elem,handler)
	{
		this.handler = handler;
		Mz.InPlaceEditors.push(this);
		this.element = elem;
		this.element.observe('click',this.beginEdit.bindAsEventListener(this));
	},
	beginEdit: function()
	{
		if (this.onEdit) return;
		this.onEdit = true;
		if (true) // this.div == null)
		{
			this.div = new Element('DIV',{"class":"MZInplaceEditor"});
			this.input = new Element("INPUT",{"type":"text"});
			var ok = new Element("IMG",{"src":"/img/ok.png"});
			var cancel = new Element("IMG",{"src":"/img/delete.png"});
			this.input.observe("blur",this.validateEdit.bindAsEventListener(this))
			this.div.insert(cancel);
			this.div.insert(this.input);
			this.div.insert(ok);
			ok.observe('click',this.validateEdit.bindAsEventListener(this));
			cancel.observe('click',this.cancelEdit.bindAsEventListener(this));
			var w = this.element.getWidth() - 40;
			this.input.setStyle({'width': w + "px" });
		}
		Mz.log('beginEdit');
		this.value = this.element.innerHTML;
		this.input.value = this.value;
		this.element.update(this.div);
		this.input.select();
	},
	cancelEdit: function(evt)
	{
		this.onEdit= false;
		Mz.log('cancelEdit :'+ this.value);
		this.element.update(this.value);
		Mz.log(this.element);
		Event.stop(evt);
	},
	validateEdit: function(evt)
	{
		Mz.log('validateEdit :'+ this.value);
		this.onEdit= false;
		Event.stop(evt);
		Mz.log(this.input.value);
		this.element.update(this.input.value);
		if (this.handler)
			this.handler(this.element,this.input.value);
	}
	
});



Mz.PeriodesTableController = Class.create(Mz.TableController,{
	url:"/ajax/periodes.php",
	
	addButton:null,
	
	initialize: function($super,elem)
		{
			$super(elem);
			this.addButton = elem.down()
		}, 
	info: function(obj)
	{
		return obj.nom + " " + obj.prenom;
	},
	
	toDate: function(ts)
	{
		Mz.log(ts);
		var x = new Date(ts * 1000);
		return x;
	},
	
	addAction: function(evt)
		{
			var x = {};
			x.code = $("periode_add_code").value;
			x.intitule = $("periode_add_intitule").value;
			x.debut = $("periode_add_debut").value;
			x.fin = $("periode_add_fin").value;
			this.addObject(x);
		},
	row: function(obj)
	{
		var tr = new Element("TR");
		var td = new Element("TD",{'MzField':'code'}).insert(obj.code); tr.insert(td);
		new Mz.InPlaceEditor(td,this.updateFieldHandler.bind(this));
		td = new Element("TD",{'MzField':'intitule'}).insert(obj.intitule); tr.insert(td);
		new Mz.InPlaceEditor(td,this.updateFieldHandler.bind(this));
		td = new Element("TD",{'MzField':'debut'}).insert(this.toDate(obj.debut).format("dd/mm/yy")); tr.insert(td);
		new XL.DatePicker(td,this.updateFieldHandler.bind(this));
		td = new Element("TD",{'MzField':'fin'}).insert(this.toDate(obj.fin).format("dd/mm/yy")); tr.insert(td);
		new XL.DatePicker(td,this.updateFieldHandler.bind(this));
		td = new Element('TD',{'class':'action'}).insert(new Element("IMG",{'src':'/img/delete.png'}));
		td.observe('click',this.deleteAction.bindAsEventListener(this));
		tr.insert(td);
		tr.mzObj = obj;
		return tr;
	},
	updateFieldHandler: function(elem,value)
	{
		Mz.log("updateHandler("+ elem +","+value+")");
		var row  = elem.up("TR");
		var obj = row.mzObj;
		obj[elem.readAttribute('MzField')] = value;
		this.updateObject(obj);
	}
});

Event.observe(document,'dom:loaded',function(){
	$$("table.PeriodesTableController").each(
		function(x){ new Mz.PeriodesTableController(x)});});

Mz.ControllerClass = Class.create({
	update : function(obj,handle)
	{
		var params = 
		new Ajax.Request("/ajax/controller.php",
			{
				"method": "POST",
				'parameters':
				{
					'action': 'update',
					'Entity': this.entity,
					'object': this.toJSON()
				}
			}
		);
	},
	onUpdate: function(resp)
	{
		
	}
})
Mz.Controller = new Mz.ControllerClass();

Mz.Object = Class.create({
	update : function()
	{
		var params = 
		new Ajax.Request("/ajax/controller.php",
			{
				"method": "POST",
				'parameters':
				{
					'action': 'update',
					'Entity': this.entity,
					'object': this.toJSON()
				}
			}
		);
	},
	onUpdate: function(resp)
	{
		
	}
})

Mz.Periode = Class.create(Mz.Object,{
	id:null,
	classe:null,
	code:'',
	intitule: '',
	debut: null,
	fin : null
});



Mz.Eleve = Class.create({
	id:null,
	classe:null,
	code:'',
	intitule: '',
	debut: null,
	fin : null
});

Mz.Periode.create = function(classe_id,code,title,debut,fin)
	{
		var x = new Mz.Periode();
		x.classe = classe_id;
		x.code = code;
		x.intitule = title;
		alert(debut);
		alert(fin);
		x.debut = Date.parserUserDate(debut);
		x.fin = Date.parserUserDate(fin);
		return x;
	}
	
	
	
XL.SelfUpdateForm = Class.create({
	form:null,
	initialize: function(frm)
		{
			this.form = frm;
			Mz.log("poppo");
			this.form.observe('submit',this.onSubmit.bindAsEventListener(this));
		},
	onSubmit: function (evt)
		{
			alert("cic");
			Event.stop(evt);
			Mz.log(this.form);
			this.form.request({"method":"POST",'onSuccess':this.onSuccess.bind(this)});
		},
	onSuccess: function(resp)
		{
			Mz.log(resp);
			this.form.update(resp.responseText);
		}
});
Event.observe(document,'dom:loaded',function(){ $$('form.XLSelfUpdateForm').each(function(x){ new XL.SelfUpdateForm(x); })});
