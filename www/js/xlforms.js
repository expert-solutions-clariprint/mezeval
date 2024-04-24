
function xl_validate_form(myform)
{
	nelems = myform.elements.length;
	for(i=0;i<nelems;i++)
	{
		elem = myform.elements[i];
		if (xl_validate_form_element(elem) == false)
		{
			xl_validate_element_alert(elem);
			return false;
		}
	}
	return true;
}

function xl_validate_element_alert(elem)
{
// ie	if (elem.hasAttribute("xl_validate_info"))
	if (elem.getAttribute("xl_validate_info") != null)
		alert(elem.getAttribute("xl_validate_info"));
	else if (elem.form.elements['xl_validate_info'] != null)
		alert(elem.form.elements['xl_validate_info'].value);
	else alert("Formulaire incomplet");
}

function xl_validate_email(str) {
	// are regular expressions supported?
	supported = 0;
	if (window.RegExp) {
		var tempStr = "a";
	var tempReg = new RegExp(tempStr);
	if (tempReg.test(tempStr)) supported = 1;
	}
	if (!supported) 
		return (str.indexOf(".") > 2) && (str.indexOf("@") > 0);
	var r1 = new RegExp("(@.*@)|(\\.\\.)|(@\\.)|(^\\.)");
	var r2 = new RegExp("^.+\\@(\\[?)[a-zA-Z0-9\\-\\.]+\\.([a-zA-Z]{2,3}|[0-9]{1,3})(\\]?)$");
	return (!r1.test(str) && r2.test(str));
}

function xl_validate_form_element(elem)
{
// don't work in IE :	if (elem.hasAttribute("xl_validate"))
	if (elem.getAttribute("xl_validate") != null)
	{
		var check_mode = elem.getAttribute("xl_validate");
		if (check_mode == 'required')
		{
			if (elem.value == '') return false;
		} else if (check_mode == 'regex')
		{
			var regex_value = elem.getAttribute("xl_validate_regex");
			var regex = new RegExp(regex_value);
			if (regex.test(elem.value)) return false;
		} else if (check_mode == 'email')
		{
			if (xl_validate_email(elem.value) == false)
			{
				return false;
			}
		} else if (check_mode == 'identity')
		{
			var identity_source = elem.getAttribute("xl_identic_at");
			var identity_obj = elem.form.elements[identity_source];
			if (identity_obj == null)
				identity_obj = document.getObjectWithId(identity_source);
			if (identity_obj == null) return false;
			if (identity_obj.value !=  elem.value)
				return false;
		} else if (check_mode == 'length')
		{
			var len = parseInt(elem.getAttribute('xl_validate_length'));
			if (elem.value.length <= len ) return false;
		}
	}
	return true;
}


function setCheckboxes(aform,regex,mode)
{
	var rgx=new RegExp(regex, "g");
	var objs = aform.getInputs('checkbox');
	var i = 0;
	for(i = 0; i < objs.length;  i++)
	{
		var cb = objs[i];
		if (cb.name.match(rgx)) cb.checked = mode;
	}
}
