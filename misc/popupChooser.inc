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
 | Filename: popupChooser.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: based on company_department_chooser.php, changed to be more     |
 | more dynamic and global by Patrick                                           |
 +------------------------------------------------------------------------------+

*/

$PHP_SELF = $PHP_SELF_MENU . "?select=" . $_REQUEST['select'] . "&amp;no_headers=1"; 

?>

<script type='text/javascript' language='javascript'>
//<![CDATA[
<!--javascript

var company_name_slot_id = '<?php print $_REQUEST['company_name_slot']; ?>'
var company_id_slot_id = '<?php print $_REQUEST['company_id_slot']; ?>'
var show_departments = '<?php print $_REQUEST['show_departments']; ?>'
var department_name_slot_id = '<?php print $_REQUEST['department_name_slot']; ?>'
var department_id_slot_id = '<?php print $_REQUEST['department_id_slot']; ?>'


function fillFields(data){
    arrNames = data.split(":");
    if (company_name_slot_id != '')     window.opener.document.getElementById(company_name_slot_id).value = arrNames[1];
    if (department_name_slot_id != '')  window.opener.document.getElementById(department_name_slot_id).value = arrNames[3];
    if (company_id_slot_id != '')       window.opener.document.getElementById(copmany_id_slot_id).value = arrNames[0];
    if (department_id_slot_id != '')    window.opener.document.getElementById(department_id_slot_id).value = arrNames[2];
    self.close();
}

function resizeOuterTo(w,h) {
    <?php
    $client = new wwwclient;
    $client->wwwclient();
    ?>
    var browser = "<?php echo $client->browser; ?>";

    var pattern = /X11/;
    var result;
    result=pattern.exec(navigator.appVersion);
    if (result != null){
        window.resizeTo((w + 320),(h + 120));
    }

    w = document.getElementById('CDC1').offsetWidth;
    h = document.getElementById('CDC1').offsetHeight;

    if (browser == <?php echo NETSCAPE; ?>){
        //w = w + 160;
        //h = h - 20;
        w = w + 30;
        h = h + 50;
    }
    if(browser == <?php echo IE; ?>) {
        //w = w + 70;
        w = w + 40;
        h = h + 30;
    }
    if(browser == <?php echo GECKO; ?>) {
        // w = w + 200;
        //h = h + 90;
    }
    if(browser == <?php echo LYNX; ?>) {
        //w = w + 150;
        //h = h + 70;
    }
    if(browser == <?php echo SAFARI; ?>) {
        //w = w + 265;
        //h = h + 45;
        w = w + 40;
        h = h + 50;
    }
    if(browser == <?php echo FIREFOX; ?>) {
        //w = w + 260;
        //h = h + 25;
        w = w + 20;
        h = h + 30;
    } else {
        //w = w + 150;
        //h = h + 100;
    }
    if(navigator.platform == "Win32") {
        w = w + 10;
        h = h + 30;
    }
    window.resizeTo(w,h);
}
//-->
//]]>
</script>
<?php
if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR){

/*
   First off, we need to find which letters to allow the user to click on across the
   screen.  If there's no companies in the database for the letter Q, there's no
   point in showing that as an option to click on.
 */

$sql = ("
        SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
        FROM employer_company
        WHERE company_name IS NOT NULL
        AND company_name!=''
        AND company_display
        ORDER BY company_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow()){
    $order_choices[] = strtoupper($row["order_letter"]);
}

if (!$order){
    $order = $order_choices[0];
}

$order_href = $PHP_SELF .= "&amp;parentFormName=" . $_REQUEST['parentFormName'];
if (isset($_REQUEST['company_name_slot'])) $order_href .= "&amp;company_name_slot=" . $_REQUEST['company_name_slot'];
if (isset($_REQUEST['company_id_slot'])) $order_href .= "&amp;company_id_slot=" . $_REQUEST['company_id_slot'];
if (isset($_REQUEST['department_name_slot'])) $order_href .= "&amp;department_name_slot=" . $_REQUEST['department_name_slot'];
if (isset($_REQUEST['department_id_slot'])) $order_href .= "&amp;department_id_slot=" . $_REQUEST['department_id_slot'];
if (isset($_REQUEST['show_departments'])) $order_href .= "&amp;show_departments=" . $_REQUEST['show_departments'];


$sql = ("
        SELECT DISTINCT ec.employer_id, ec.company_name,concat(ed.department_name,\" (\",ed.city,\")\") as d_name,ed.department_name,ed.department_id, ed.city,
        ec.city as eccity, concat(ec.company_name,\" (\",ec.city,\")\") as c_name
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
$result = $GLOBALS['dbh']->Execute($sql);
?>


<form name='form1'>

<center>
<table border='0' width='100%' height='100%' align='center' valign='middle'>
  <tr align='center'>
    <td align='center'>
      <table id='CDC1' name='mainTable' border='0' cellpadding='5' cellspacing='0' class='row1'>
        <tr>
          <td colspan='2'>
            <br />
          </td>
        </tr>
        <tr>
          <td align='center' colspan='2' nowrap='nowrap'>
        Choose a company:</td>
        </tr>
        <tr>
          <td colspan='2'>
            <table width='100%' cellpadding='5' cellspacing='5' border='0'>
              <tr>
                <td colspan='2' class='row0'>
                <table width='96%' cellpadding='2' cellspacing='0' border='0'>
                  <tr>
                    <td colspan='2' align='center' nowrap='nowrap'>


<?php
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
if ($result->RecordCount() < 10){
    $size = $result->RecordCount() + 1;
}
if ($result->RecordCount() > 0){
    echo("<select name='company_name' onChange='fillFields(document.form1.company_name[document.form1.company_name.selectedIndex].value)' size='" . $size . "'>");
    echo("<option value='' ");
    if (!$company_name || $company_name == ''){
        echo("selected");
    }
    echo(">&nbsp;</option>");
    while ($row = $result->FetchRow()){
        $company_string = $row['employer_id'] . ":" . htmlentities($row["company_name"]); 
        if ($lastCompanyName<>$row['company_name']){	
            echo("<option value=\"" . $company_string . ": : \" ");
            if ($employer_id == $row["company_name"]){
                echo("selected");
            }
            echo ">" . $row["company_name"] . "</option>";
        }
        if ($show_departments == 1){
            // For department names, we concatenate a city to the department names. However, concatentation fails if we try to
            // concatenate a NULL city. Therefore, we check to make sure that the city is not null, and we provide additional
            // cases to handle this problem.
            if ($row['department_id']<>NULL && $row['city']<>NULL){
                echo ("<option value=\"". $company_string . ":" . $row['department_id'] . ":" . htmlentities($row['department_name'])."\">&nbsp;&nbsp;-".$row['d_name']."</option>");
            }
            elseif ($row['department_name'] <> NULL) {
                echo ("<option value=\"". $company_string . ":" . $row['department_id'] . ":" . htmlentities($row['department_name'])."\">&nbsp;&nbsp;-".$row['department_name']."</option>");
            }
            elseif ($row['employer_id'] <> NULL && $row['eccity'] <> NULL) {
                echo ("<option value=\"" . $copmany_string . ":" . $row['department_id'] . ":" . htmlentities($row['company_name'])."\">&nbsp;&nbsp;-".$row['c_name']."</option>");
            }
            elseif ($row['company_name'] <> NULL) {
                echo ("<option value=\"" . $copmany_string . ":" . $row['department_id'] . ":" . htmlentities($row['company_name'])."\">&nbsp;&nbsp;-".$row['company_name']."</option>");
            }
        }
        $lastCompanyName=$row['company_name'];
    }
    echo("</select>");
} else {
    echo("<br />No companies starting with '" . $order . "'.<br /><br />");
}
?>
                      </td>
                    </tr>
                  </table>
                </td>
              </tr>
            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</center>
</form>
<?php } ?>

<script type='text/javascript' language='javascript'>
//<!CDATA[
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

widthOffSet = document.getElementById('CDC1').offsetWidth;
heightOffSet = document.getElementById('CDC1').offsetHeight;

resizeOuterTo((widthOffSet),(heightOffSet));
//]]>
</script>
