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
 | Filename: company_department_choose_3.php                                    |
 +------------------------------------------------------------------------------+
 | Description: based on company_choose.php, this file allows you to choose a   |
 | department as well as a department.  Not only is the name filled in for the  |
 | parent form, but another field (typically hidden) is filled with the         |
 | division or company id (prefixed with either comp or div). Also, this file   |
 | CAN (but doens't have to) be passed a variable called withContacts, which,   |
 | if set to true, will cause only companies and divisions with contacts under  |
 | them to be displayed.                                                        |
 +------------------------------------------------------------------------------+

*/


if ($withContacts)
{
    $sql = ("
            SELECT DISTINCT SUBSTRING(ec.company_name, 1, 1) AS order_letter
            FROM employer_company AS ec, contact_employer AS ce
            WHERE ce.employer_id=ec.employer_id
            AND ec.company_name IS NOT NULL
            AND ec.company_name!=''
            AND ec.company_display
            ORDER BY ec.company_name
            ");
}
else
{
    $sql = ("
            SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
            FROM employer_company
            WHERE company_name IS NOT NULL
            AND company_name!=''
            AND company_display
            ORDER BY company_name
            ");
}
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

// Create company_name_array
$str = ("var company_name_array = new Array();");

if ($withContacts)
{
    $sql = ("
            SELECT DISTINCT ec.company_name, ec.employer_id
            FROM employer_company AS ec, contact_employer AS ce
            WHERE ce.employer_id=ec.employer_id
            AND ec.company_display
            AND ec.company_name LIKE '" . $order . "%'
            ORDER BY ec.employer_id
            ");
}
else
{
    $sql = ("
            SELECT DISTINCT company_name, employer_id
            FROM employer_company
            WHERE company_display
            AND company_name LIKE '" . $order . "%'
            ORDER BY employer_id
            ");
}
$result = $GLOBALS['dbh']->Execute($sql);
// getting employer_ids
$arr_employer_id = array();

while ($row = $result->FetchRow())
{
    $str2 .= ("company_name_array[" . $row["employer_id"] . "] = \\\"" .htmlentities( $row["company_name"]) . "\\\"; ");
}

$company_array_string = $str . $str2;

unset($str);
unset($str2);

// Create division_name_array
$str = ("var division_name_array = new Array();");

/*Commented out: Jon
  Previous change that Chris made to fix "H" problem. Leave here
  just in case.

  $list_of_employer_id = "('".implode("','",$arr_employer_id)."')";

  $sql = ("
  SELECT ed.department_name, ed.department_id
  FROM employer_department AS ed
  INNER JOIN employer_company AS ec
  ON ed.employer_id = ec.employer_id
  INNER JOIN contact_employer AS ce
  ON ce.department_id = ed.department_id
  WHERE ec.company_display = 1 AND ec.employer_id = '14405'
  ORDER BY ed.department_id
  ");
//IN ".$list_of_employer_id."

 */
if ($withContacts)
{
    $sql = ("
            SELECT DISTINCT ed.department_name, ed.department_id
            FROM employer_department AS ed, employer_company AS ec, contact_employer AS ce
            WHERE ed.employer_id=ec.employer_id
            AND ce.department_id=ed.department_id
            AND ec.company_display
            AND ec.company_name LIKE '" . $order . "%'
            ORDER BY ed.department_id
            ");
}
else
{
    $sql = ("
            SELECT DISTINCT ed.department_name, ed.department_id
            FROM employer_department AS ed, employer_company AS ec
            WHERE ed.employer_id=ec.employer_id
            AND ec.company_display
            AND ec.company_name LIKE '" . $order . "%'
            ORDER BY ed.department_id
            ");
}
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    $str2 .= ("division_name_array[" . $row["department_id"] . "] = \\\"" . htmlentities($row["department_name"]) . "\\\"; ");
}

$division_array_string = $str . $str2;
?>

<script type='text/javascript' language='javascript'>
<!--javascript

// Create the needed arrays
eval("<?php echo($division_array_string); ?>");
eval("<?php echo($company_array_string); ?>");

function unfocus()
{
    self.close();
}

// is this ok?
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

function fillValues(compDivId)
{
    var compOrDiv = compDivId.substr(0,4);
    var idNumber = compDivId.substr(4);

    var x = document.form1.company_slot.value;
    var y = document.form1.id_slot.value;
    var formName = document.form1.parentFormName.value;

    if (compOrDiv == 'dept')
    {
        
   //     Commented by: Jon
   //     Reason: This was allowing the user to choose a company with division.
    
    //if (compOrDiv == 'comp')
    //{
      //  var name = translateEntity(company_name_array[idNumber]);
    //}
    //else
    //{
        var name = translateEntity(division_name_array[idNumber]) + " (Division)";
    //}

        str = "window.opener.document." + formName + ".elements[" + x + "].value = name";
        str2 = "window.opener.document." + formName + ".elements[" + y + "].value = compDivId";

        eval(str);
        eval(str2);
        self.close();
    }
}

function resizeOuterTo(w,h) 
{
    <?php
    $client = new wwwclient;
    $client->wwwclient();
    ?>
    var browser = "<?php echo $client->browser; ?>";

    var pattern = /X11/;
    var result;
    result=pattern.exec(navigator.appVersion);
    if (result != null)
    {
        w = w + 320;
        h = h + 120;
    }
    // will dynamically retrieve the window's height and width
    w = document.getElementById('CDC3').offsetWidth;
    h = document.getElementById('CDC3').offsetHeight;
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

$order_href = ("mamook.php?select=company_department_choose_3&no_headers=1&company_slot=" . $company_slot . "&id_slot=" . $id_slot . "&parentFormName=" . $parentFormName);

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{

    if ($withContacts)
    {
        $sql = ("
                SELECT DISTINCT ec.employer_id, ec.company_name, CONCAT(ed.department_name, \" (\",ed.city,\")\") AS d_name, ed.department_name, ed.department_id, ed.city
                FROM employer_company AS ec, contact_employer AS ce
                LEFT JOIN employer_department AS ed
                ON (ed.employer_id=ec.employer_id AND ce.department_id=ed.department_id)
                WHERE ec.company_name IS NOT NULL
                AND ce.employer_id=ec.employer_id
                AND ec.company_display='1'
                AND ec.company_name!=''
                AND ec.company_name LIKE '" . $order . "%'
                AND ec.company_display
                ORDER BY ec.company_name, ed.department_name
                ");
    }
    else
    {
        $sql = ("
                SELECT DISTINCT ec.employer_id, ec.company_name,concat(ed.department_name,\" (\",ed.city,\")\") as d_name,ed.department_name,ed.department_id, ed.city
                FROM employer_company as ec
                LEFT JOIN employer_department as ed
                ON ed.employer_id=ec.employer_id
                WHERE ec.company_name IS NOT NULL
                AND ec.company_display=1
                AND ec.company_name!=''
                AND ec.company_name LIKE '" . $order . "%'
                AND ec.company_display
                ORDER BY ec.company_name, ed.department_name
                ");
    }
    $result = $GLOBALS['dbh']->Execute($sql);

    // Figure out why this isn't working for oldCompany_oldContact now.
    echo("</head><body bgcolor='ffffff'><form name='form1'>");

    echo("<input type='hidden' name='company_slot' value='" . $company_slot . "'>");
    echo("<input type='hidden' name='id_slot' value='" . $id_slot . "'>");
    echo("<input type='hidden' name='parentFormName' value='" . $parentFormName . "'>");

    echo("<center>");
    echo("<table border='0' width='100%' height='100%' align='center' valign='middle'>");
    echo("<tr align='center'>");
    echo("<td align='center'>");
    echo("<table id='CDC3' name='mainTable' border='0' cellpadding='5' cellspacing='0' class='row1'>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<br />");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap>Choose a Company/Division:</td>");
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
    if ($result->RecordCount() < 7)
    {
        /* 
           Take into account departments AND companies appearing on the same row, so add another half 
           times the number of rows to the number of rows for size.
         */
        $size = $result->RecordCount() + floor($result->RecordCount()/2) + 1;
    }
    if ($result->RecordCount() > 0)
    {
        echo("<select name='company_name' onChange='fillValues(document.form1.company_name[document.form1.company_name.selectedIndex].value)' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$company_name || $company_name == '')
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            if ($lastCompanyName<>$row['company_name'])
            {	
                echo("<option value=\"comp" . $row["employer_id"] . "\" ");
                if ($employer_id == $row["company_name"])
                {
                    echo("selected='selected'");
                }
                echo(">");
                echo ($row["company_name"] . "</option>");
            }
            // If the department name is concatenated with a NULL city, the concatentation will fail. Therefore, the elseif
            // statement handles this problem.
            if ($row['department_id']<>NULL && $row['city']<>NULL)
            {
                if (preg_match("/.*\(\)/", trim($row["d_name"])))
                {
                    $row["d_name"] = substr($row["d_name"], 0, -3);
                }
                echo ("<option value=\"dept" . $row["department_id"] . "\">&nbsp;&nbsp;-".$row['d_name']."</option>");
            }
            elseif ($row['department_name'] <> NULL) {
                echo ("<option value=\"dept" . $row["department_id"] . "\">&nbsp;&nbsp;-".$row['department_name']."</option>");
            }
            $lastCompanyName=$row['company_name'];
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
}  else {
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

//resizeOuterTo((285 + widthOffSet),(175 + heightOffSet));

widthOffSet = document.getElementById('CDC3').offsetWidth;
heightOffSet = document.getElementById('CDC3').offsetHeight;

resizeOuterTo(widthOffSet,heightOffSet);

</script>
</body>
</html>
<?php
ob_end_flush();
?>
