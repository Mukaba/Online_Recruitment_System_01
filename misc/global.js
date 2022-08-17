
//Function modified by Adam to allow for the passing of the target slot and form as parameters.
function popup(slotName, formName)
{
	var str = "document." + formName + ".elements";
	var j = eval(str);
 	var pattern = /X11/;
  	var result;
	for (var i = 0; i < j.length; i++)
	{
		var e = j[i];
		if (e.name == slotName)
		{
			var slot = i;
			break;
		}
	}
        result=pattern.exec(navigator.appVersion);
	if (result != null)
	{
	window.open("mamook.php?select=popupcalendar&no_headers=1&index="+slot+"&parentFormName="+formName, "Calendar", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,height=500,width=350");
	} else {
	window.open("mamook.php?select=popupcalendar&no_headers=1&index="+slot+"&parentFormName="+formName, "Calendar", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,height=275,width=275");
	}
}

function popUpChooser(company_name_slot, company_id_slot, department_name_slot, department_id_slot){
    var winstring = "mamook.php?select=popupChooser&no_headers=1";
    if (company_name_slot != '' && company_name_slot != undefined)
        winstring = winstring + '&company_name_slot=' + company_name_slot;
    if (company_id_slot != '' && company_id_slot != undefined)
        winstring = winstring + '&company_id_slot=' + company_id_slot;
    if (department_name_slot != '' && department_name_slot != undefined)
        winstring = winstring + '&department_name_slot=' + department_name_slot;
    if (department_id_slot != '' && department_id_slot != undefined)
        winstring = winstring + '&department_id_slot=' + department_id_slot;
    if ((department_id_slot != '' && department_id_slot != undefined) || (department_name_slot != '' && department_name_slot != undefined))
        winstring = winstring + '&show_departments=1';
    
    window.open(winstring, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes");
}

