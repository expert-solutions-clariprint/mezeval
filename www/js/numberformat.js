(function(){ 
	var mil_delim = ' ';
	var dec_delim = '.';
	var init = function(){
		$$('.numberformat').each(function(field){
			field.observe('blur', numberformat);
			numberformat_currency(field);
		});
	};
	var numberformat = function(evt){
		numberformat_currency(evt.element());
	};
	var numberformat_currency = function(element){
//		var element = e.element();
		var val = element.value;
		val = val.replace(",",".");
		val = val.replace(" ","");
		var arr = val.split(dec_delim);
		var aint = arr.first();
		aint	 = aint.strip().gsub(mil_delim, '');
		var dec  = dec_delim;
		val = currency_formatter(aint) ;
		if (arr.size() > 1 && !element.hasClassName('integer'))  val += dec_delim + arr.last();
		element.value = val;
	};
	var currency_formatter = function(aint){
		//first reverse array (to work from back), then slice in 3
		// for each group, reverse and join the 3 numbers 
		// Reverse the direction of the groups and then add thousand separators between
		return aint.toArray().reverse().eachSlice(3, function(group){return group.reverse().join('')}).reverse().join(mil_delim);
	};
	$(document).observe("dom:loaded", function() { init(); }); })();


function parseFormatedInt(val)
{
	return parseInt(val.replace(" ",""));
}