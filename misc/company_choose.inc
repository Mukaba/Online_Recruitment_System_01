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
 | Filename: company_choose.php                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file allows the user to choose a company from a list       |
 | shown in a seperate pop-up window.  Once selected, the company name is sent  |
 | back to the window that opened this file, and this files window is closed.   |
 +------------------------------------------------------------------------------+

*/
?>
<script type='text/javascript' language='javascript'>

<!--javascript

function unfocus()
{
        self.close();
}

function replaceString(string, regExp, changed)
{   
    var re = new RegExp (regExp, 'gi') ;
    var newstr = string.replace(re, changed);
    return newstr;
}   

function translateEntity(entityString)
{   
    entityString = replaceString(entityString, '&amp;', '&');
    entityString = replaceString(entityString, '&#039;', '\'');
    entityString = replaceString(entityString, '&quot;', '\"');
    entityString = replaceString(entityString, '&lt;', '<');
    entityString = replaceString(entityString, '&gt;', '>');
    return entityString;
}   

function fillCompanyName(companyName)
{
    // to remove the html entities
    companyName = translateEntity(companyName);
    var x = document.form1.company_slot.value;
    var formName = document.form1.parentFormName.value;

    str = "window.opener.document." + formName + ".elements[" + x + "].value = companyName";
    eval(str);
    self.close();
}

function resizeOuterTo(w,h) 
{
 <?php
 
 $client = new wwwclient;
 $client->wwwclient();
 ?>
 var pattern = /X11/;
 var result;
 var browser = <?php echo $client->browser; ?>;
 	result=pattern.exec(navigator.appVersion); 
 	if (result != null) 
	{
	window.resizeTo((w + 320),(h + 150));
	}
    
    w = document.getElementById('CC').offsetWidth;
    h = document.getElementById('CC').offsetHeight;

    // Netscape
    if (browser == <?php echo NETSCAPE; ?>)
    {
        //w = w + 80;
        //h = h;
        w = w + 20;
        h = h + 40;
    }
    // IE
    if(browser == <?php echo IE; ?>)
    {
        //w = w + 70;
        w = w + 20;
        h = h + 20;
    }
    // Gecko/Netscape/Mozilla
    if(browser == <?php echo GECKO; ?>)
    {
        //w = w + 260;
        //h = h + 120;
        h = h + 10;
    }
    // Lynx
    if(browser == <?php echo LYNX; ?>)
    {
        //w = w + 150;
        //h = h + 100;
    }
    // Safari
    if(browser == <?php echo SAFARI; ?>)
    {
        //w = w + 275;
        //h = h + 30;
        w = w + 25;
        h = h + 30;
    }
    // Firefox/bird
    if(browser == <?php echo FIREFOX; ?>)
    {
        //w = w + 290;
        //h = h + 35;
        w = w + 25;
        h = h + 50;
    }
    else
    {
        //w = w + 150;
        //h = h + 120;            
    }
    if(navigator.platform == "Win32")
    {
        h = h + 20;
    }
    window.resizeTo(w,h);
}



//-->

</script>
<?php

/*
 First off, we need to find which letters to allow the user to click on across the
 screen.  If there's no companies in the database for the letter Q, there's no
 point in showing that as an option to click on.
*/

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{

    $sql = ("
            SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
            FROM employer_company
            WHERE company_name IS NOT NULL
            AND company_name!=''
            AND company_display
            ORDER BY company_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
            $order_choices[] = strtoupper($row["order_letter"]);
    }

    if (!$order)
    {
            $order = $order_choices[0];
    }

    $order_href = ("mamook.php?select=company_choose&no_headers=1&company_slot=" . $company_slot . "&parentFormName=" . $parentFormName);

$sql = ("
        SELECT DISTINCT employer_id, company_name
        FROM employer_company
        WHERE company_name IS NOT NULL
        AND company_name!=''
        AND company_name LIKE '" . $order . "%'
	AND company_display
        ORDER BY company_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

echo("</head><body bgcolor='#ffffff'><form name='form1'>");

echo("<input type='hidden' name='company_slot' value='" . $company_slot . "'>");
echo("<input type='hidden' name='parentFormName' value='" . $parentFormName . "'>");

echo("<center>");
echo("<table border='0' width='100%' height='100%' align='left' valign='top'>");
echo("<tr align='center'>");
echo("<td align='center'>");
echo("<table id='CC' name='mainTable' border='0' cellpadding='5' cellspacing='0' class='row1'>");

echo("<tr>");
        echo("<td colspan='2'>");
        echo("<br />");
        echo("</td>");
echo("</tr>");

echo("<tr>");
        echo("<td align='center' colspan='2' nowrap>Choose a company:</td>");
echo("</tr>");
echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table cellpadding='5' cellspacing='5' border='0'>");
        echo("<tr>");
                echo("<td colspan='2' class='row0'>");
                echo("<table width='100%' cellpadding='2' cellspacing='0' border='0'>");
		echo("<tr>");
                        echo("<td colspan='2' align='center' nowrap>");
                        for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
                        {
                                echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
                        }
                        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");

			/*
			 Input below is used to tell javascript the size of the order letters array.
			*/

			echo("<input type='hidden' name='order_width' value='" . sizeof($order_choices) . "'>");
                        echo("</td>");
                echo("</tr>");
                echo("<tr>");
                        echo("<td colspan='2'  align='center'>");
                        $size = 10;
                        if ($result->RecordCount() < 10)
                        {
                                $size = $result->RecordCount() + 1;
                        }
                        if ($result->RecordCount() > 0)
                        {
                                echo("<select name='company_name' onChange='fillCompanyName(document.form1.company_name[document.form1.company_name.selectedIndex].value)' size='" . $size . "'>");
                                echo("<option value='' ");
                                if (!$company_name || $company_name == '')
                                {
                                        echo("selected");
                                }
                                echo(">&nbsp;</option>");
                                while ($row = $result->FetchRow())
                                {
                                        echo("<option value=\"" . htmlentities($row["company_name"]) . "\" ");
                                        if ($employer_id == $row["company_name"])
                                        {
                                                echo("selected");
                                        }
                                        echo(">" . $row["company_name"] . "</option>");
                                }
                                echo("</select>");
                        }
                        else
                        {
                                echo("<br />No companies starting with '" . $order . "'.<br /><br />");
                        }
                        echo("</form>");
                        echo("</td>");
                echo("</tr>");
                echo("</table>");
                echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
echo("</tr>");
echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</center>");
} else {
    echo("</head><body bgcolor='#ffffff'>");
}

?>

<script type='text/javascript' language='javascript'>

var heightOffSet = 0;
var widthOffSet = 0;

var widthMultiplier = 8;
var heightMultiplier = 10;

var selectLength = 0;

selectLength = (document.form1.company_name.length - 2);

if (selectLength > 10)
{
	selectLength = 10;
}

var selectWidth = 0;

var i;

for (i = 0; i < document.form1.company_name.length; i++)
{
	curLength = document.form1.company_name[i].text.length;
	if (curLength > selectWidth)
	{
		selectWidth = curLength;
	}
}

selectWidth = selectWidth - 30;

if (selectWidth < 0)
{
	selectWidth = 0;
}

if (document.form1.order_width.value > selectWidth)
{
	selectWidth = document.form1.order_width.value;
	widthMultiplier = 9.0;
}

//widthOffSet = selectWidth * widthMultiplier;
//heightOffSet = selectLength * heightMultiplier;

widthOffSet = document.getElementById('CC').offsetWidth;
heightOffSet = document.getElementById('CC').offsetHeight;
    
resizeOuterTo((widthOffSet),(heightOffSet));

</script>
</body>
</html>
<?php
    ob_end_flush();
?>
