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
 | Filename: contact_choose.php                                                 |
 +------------------------------------------------------------------------------+
 | Description: based on company_choose.php, this file allows you to choose a   |
 | contact that matches the supplied employer or division id. Not only is the   |
 | name filled in for the parent form, but another field (typically hidden) is  |
 | filled with the contact id.                                                  |
 +------------------------------------------------------------------------------+

*/

if ($compOrDiv == '-1')
{
    echo("<center>");
    echo("<form>");
    echo("<table border='0' width='100%' height='100%' align='center' valign='middle'>");
    echo("<tr align='center'>");
        echo("<td align='center'>");
        echo("Please choose a company/division first.");
        echo("</td>");
    echo("</tr>");
    echo("<tr align='center'>");
        echo("<td align='center'>");
        echo("<input type='button' value='Close this window' onClick='unfocus();'>");
        echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</form>");
    echo("</center>");
}
else
{
    $sql = ("
        SELECT DISTINCT SUBSTRING(c.last_name, 1, 1) AS order_letter
        FROM contact AS c, contact_employer AS ce
        WHERE c.contact_id=ce.contact_id
        AND c.last_name!=''
        ");
    if ($compOrDiv == 'comp')
    {
        $sql .= ("
            AND ce.employer_id='" . $compDivId . "'
            ");
    }
    elseif ($compOrDiv == 'dept')
    {
        $sql .= ("
            AND ce.department_id='" . $compDivId . "'
            ");
    }

    $sql .= ("
        ORDER BY c.last_name
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        if (!is_array($order_choices) || !in_array(strtoupper($row["order_letter"]), $order_choices))
        {
            $order_choices[] = strtoupper($row["order_letter"]);
        }
    }

    if (!$order)
    {
        $order = $order_choices[0];
    }

    $sql = ("
        SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, c.contact_id
        FROM contact AS c, contact_employer AS ce
        WHERE c.contact_id=ce.contact_id
        AND c.last_name LIKE '" . addslashes($order) . "%'
        ");

    if ($compOrDiv == 'comp')
    {
        $sql .= ("
            AND ce.employer_id='" . $compDivId . "'
            ");
    }
    elseif ($compOrDiv == 'dept')
    {
        $sql .= ("
            AND ce.department_id='" . $compDivId . "'
            ");
    }
    $result = $GLOBALS['dbh']->Execute($sql);

    $str = ("var contact_name_array = new Array(); ");

    while ($row = $result->FetchRow())
    {
        $str .= ("contact_name_array[" . $row["contact_id"] . "] = \\\"" . $row["name"] . "\\\"; ");
    }

    $contact_array_string = $str . $str2;
    ?>

    <script type='text/javascript' language='javascript'>
    <!--javascript

    // Create the needed arrays
    eval("<?php echo($contact_array_string); ?>");

    function fillValues(contactId)
    {
        var x = document.form1.contact_name_slot.value;
        var y = document.form1.id_slot.value;
        var formName = document.form1.parentFormName.value;

        var name = contact_name_array[contactId];

        str = "window.opener.document." + formName + ".elements[" + x + "].value = name";
        str2 = "window.opener.document." + formName + ".elements[" + y + "].value = contactId";

        eval(str);
        eval(str2);
        self.close();
    }

    function resizeOuterTo(w,h) 
    {
        <?php 
        $client = new wwwclient;
        $client->wwwclient();
        $browser = $client->browser;
        ?>
        
        var browser = "<?php echo $browser; ?>";
        var pattern = /X11/;
        var result;
        result=pattern.exec(navigator.appVersion);
        if (result != null)
        {
            w = w + 320;
            h = h + 120;
        } 
        // will dynamically retrieve the window's height and width
        w = document.getElementById('CONC').offsetWidth;
        h = document.getElementById('CONC').offsetHeight;
        if(browser == <?php echo NETSCAPE; ?>)
        {
            //w = w + 70;
            w = w + 30;
            h = h + 40;
        }
        if(browser == <?php echo IE; ?>)
        {
            //w = w + 70;
            //h = h + 20;
            w = w + 40;
            h = h + 30;
        }
        if(browser ==  <?php echo GECKO; ?>)
        {
            //w = w + 200;
            //h = h + 140;
        }
        if(browser ==  <?php echo LYNX; ?>)
        {
            //w = w + 150;
            //h = h + 140;
        }
        if(browser ==  <?php echo SAFARI; ?>)
        {
            //w = w + 265;
            //h = h + 70;
            w = w + 40;
            h = h + 70;
        }
        if(browser ==  <?php echo FIREFOX; ?>)
        {
            //w = w + 260;
            //h = h + 25;
            w = w + 30;
            h = h + 30;
        }
        else
        {
            w = w + 150;                                                                                                                               
            h = h + 120;                                                                                                                               
        }                                                                                                                                                
        if(navigator.platform == "Win32")                                                                                                                
        {                                                                                                                                                
            h = h + 40;                                                                                                                                  
        }
        //w = w + 20;
        //h = h + 40;
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

    $order_href = ("mamook.php?select=contact_choose&no_headers=1&contact_name_slot=" . $contact_name_slot . "&compOrDiv=" . $compOrDiv . "&compDivId=" . $compDivId . "&id_slot=" . $id_slot . "&parentFormName=" . $parentFormName);

    $sql = ("
        SELECT DISTINCT CONCAT(c.last_name, ', ', c.first_name) AS name, c.contact_id
        FROM contact AS c, contact_employer AS ce
        WHERE c.contact_id=ce.contact_id
        AND c.last_name LIKE '" . addslashes($order) . "%'
        ");

    if ($compOrDiv == 'comp')
    {
        $sql .= ("
            AND ce.employer_id='" . $compDivId . "'
            ");
    }
    elseif ($compOrDiv == 'dept')
    {
        $sql .= ("
            AND ce.department_id='" . $compDivId . "'
            ");
    }
    $result = $GLOBALS['dbh']->Execute($sql);

    echo("<form name='form1'>");

    echo("<input type='hidden' name='contact_name_slot' value='" . $contact_name_slot . "'>");
    echo("<input type='hidden' name='id_slot' value='" . $id_slot . "'>");
    echo("<input type='hidden' name='compOrDiv' value='" . $compOrDiv . "'>");
    echo("<input type='hidden' name='compDivId' value='" . $compDivId . "'>");
    echo("<input type='hidden' name='parentFormName' value='" . $parentFormName . "'>");

    echo("<center>");
    echo("<table border='0' width='100%' height='100%' align='center' valign='middle'>");
    echo("<tr align='center'>");
    echo("<td align='center'>");
    echo("<table id='CONC' name='mainTable' border='0' cellpadding='5' cellspacing='0' class='row1'>");

    echo("<tr>");
            echo("<td colspan='2'>");
            echo("<br />");
            echo("</td>");
    echo("</tr>");

    echo("<tr>");
            echo("<td align='center' colspan='2' nowrap>Choose a Contact:</td>");
    echo("</tr>");
    echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
        echo("<tr>");
            echo("<td colspan='2' class='row0'>");
            echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
            echo("<tr>");
                echo("<td colspan='2' align='center' nowrap>");
                for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
                {
                        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
                }
                echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");

                // Input below is used to tell javascript the size of the order letters array.

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
                    echo("<select name='contact_id' onChange='fillValues(document.form1.contact_id[document.form1.contact_id.selectedIndex].value)' size='" . $size . "'>");
                    echo("<option value='' ");
                    if (!$contact_id || $contact_id == '')
                    {
                        echo("selected='selected'");
                    }
                    echo(">&nbsp;</option>");
                    while ($row = $result->FetchRow())
                    {
                        echo("<option value='" . $row["contact_id"] . "'");
                        if ($contact_id == $row["contact_id"])
                        {
                            echo(" selected='selected'");
                        }
                        echo(">" . $row["name"] . "</option>");
                    }
                    echo("</select>");
                }
                else
                {
                    echo("<br />No contacts starting with '" . $order . "', at the selected company/division.<br />");
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
}

?>

<script type='text/javascript' language='javascript'>

var heightOffSet = 0;
var widthOffSet = 0;

var widthMultiplier = 8;
var heightMultiplier = 10;

var selectLength = 0;

selectLength = (document.form1.contact_id.length - 2);

if (selectLength > 10)
{
	selectLength = 10;
}

var selectWidth = 0;

var i;

for (i = 0; i < document.form1.contact_id.length; i++)
{
	curLength = document.form1.contact_id[i].text.length;
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


widthOffSet = document.getElementById('CONC').offsetWidth;                                                                                             
heightOffSet = document.getElementById('CONC').offsetHeight;     

resizeOuterTo((widthOffSet),(heightOffSet));


</script>
