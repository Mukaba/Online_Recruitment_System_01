<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: view_company_criteria_simple.inc                                   |
 +------------------------------------------------------------------------------+
 | Description: This is the file called up when the user wants a simpler        |
 | method to search on companies.  This set of criteria allows them only to     |
 | search on company names.                                                     |
 +------------------------------------------------------------------------------+

*/


?>

<script type="text/javascript" language="javascript">
<!--javascript

function popUpCompany(slotName, formName)
{
	var str = "document." + formName + ".elements";

	var j = eval(str);
	for (var i = 0; i < j.length; i++)
	{
		var e = j[i];
		if (e.name == slotName)
		{
			var slot = i;
			break;
		}
	}

	window.open("mamook.php?select=company_department_choose_2&no_headers=1&company_slot="+slot+"&parentFormName="+formName, "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes, height=50");
}

//-->
</script>

<?php

echo("<table width='100%' border='0' cellpadding='0' cellspacing='0'>");
echo("<tr>");
	echo("<td>");
echo("<form method='post' name='myform' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_contact' />");
echo("<input type='hidden' name='level1' value='company' />");

echo("<table border='0' cellpadding='8' cellspacing='0' width='97%' class='row1'>");

echo("<tr>");
	echo("<td align='center' colspan='2'>");
	echo("<input type='button' value='Go to Advanced Search' onclick='javascript:document.advanced_search_form.submit();' />");
	echo("</td>");
echo("</tr>");

echo("<tr>");
	echo("<td align='center' colspan='2'><b class='black'>");
	echo("<br />The list of companies and divisions returned will match the criteria entered below.  ");
	echo("<br />Leave any fields blank that you do not wish to be used in the search.");
	echo("</b></td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'><hr width='50%' size='1' /></td>");
echo("</tr>");

echo("<tr>");
    echo("<td align='right'>Company/Division name: ");
    echo("<input type='radio' class='row1' name='search_name_as' value='start'");
    echo(($search_name_as == 'start' || !$search_name_as) ? "checked='checked'" : "");
    echo(" />Starting with&nbsp;");

    echo("<input type='radio' class='row1' name='search_name_as' value='containing'");
    echo(($search_name_as == 'containing') ? "checked='checked'" : "");
    echo(" />Containing&nbsp;");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' name='search_name_m' size='30'");
    if ($search_name_m)
    {
            echo(" value=\"" . addslashes(trim($search_name_m)) . "\"");
    }
    echo(" />");

    echo("</td>");
echo("</tr>");

$sql = ("
        SELECT DISTINCT employer_id
        FROM employer_company
        WHERE company_display
        "); 
$result = $GLOBALS['dbh']->Execute($sql); 
        
if ($result->RecordCount())
{       
    echo("<tr>");
            echo("<td colspan='2' align='center'>-Or-</td>");
    echo("</tr>");

    echo("<tr>");
            echo("<td colspan='2' align='center'>");
            echo("<input type='button' name='company_list' value='Choose from a list' onclick='popUpCompany(\"search_name_m\", \"myform\");' />");
            echo("</td>");
    echo("</tr>");
}                       

echo("<tr>");
        echo("<td colspan='2' align='center'><hr />");
        echo("<input type='hidden' name='display1' value='name' />");
        echo("<input type='hidden' name='display2' value='phone' />");
        echo("<input type='hidden' name='display3' value='email' />");
        echo("<input type='hidden' name='continue' value='first_company_form_done' />");
        echo("<input type='submit' value='Continue' />");
        echo("&nbsp;&nbsp;&nbsp;");
        echo("<input type='reset' value='Reset Form' />");
        echo("</td>");
echo("</tr>");

echo("</table>");

echo("</form>");
        echo("</td>");
echo("</tr>");
echo("</table>");
echo("<form method='post' name='advanced_search_form' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_contact' />");
echo("<input type='hidden' name='level1' value='company' />");
echo("<input type='hidden' name='switch_search' value='Go to Advanced Search' />");
echo("</form>");
