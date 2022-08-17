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
 | Filename: main_form.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file is the main form for adding a history record.         |
 +------------------------------------------------------------------------------+

*/
?>

<script type="text/javascript" language="javascript">
<!--javascript
function popUpStudent(slotName, formName) {

    var str = "document." + formName + ".elements";

    var j = eval(str);

    for (var i = 0; i < j.length; i++) {
        var e = j[i];

        if (e.name == slotName) {
            var slot = i;
            break;
        }   
    }

    window.open("mamook.php?select=student_chooser2&no_headers=1&student_list_slot="+slot+"&parentFormName="+formName, "StudentChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,width=285,height=175");
}

//-->   
</script>
<?php

// Display appropriate header: 'own job placement' and 'returning placement' function synonymously; only diff between them is header and page re-routes
if ($select == "add_history")
{
    echo("<h3 align='left'>Job Offers - Own Job Placement</h3>");
}
if ($select == "returning_placement")
{
    echo("<h3 align='left'>Job Offers - Returning Job Placement</h3>");
}

// Ensure record id is an integer. This is for incoming record ids from the student quickmenu. 
if ($record_id && preg_match("/^\d+$/",$record_id))
{
    $sql = ("
            SELECT student_number
            FROM   student_department
            WHERE  record_id = '".$record_id."'
            ");
    $results     = $GLOBALS['dbh']->Execute($sql);
    $row         = $results->FetchRow();
    $student_num = $row['student_number'];
}

if ($addHistory->student_number) 
{
    $student_number = $addHistory->student_number;
}
// coming from quick menu
elseif (!$addHistory->student_number && $student_num != "-1")
{
    $student_number = $student_num;
}

if ($error_array && sizeof($error_array))
{
    foreach ($error_array as $error_msg)
    {
        error($error_msg);
    }
    echo("<br />");
}
echo("<form method='post' action='".$PHP_SELF."' name='myform'>");
echo("<table class='row1' cellpadding='4' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td>Company Name:</td>");
echo("<td><input type='text' size='50' name='company_name' id='company_name' value='".stripslashes($addHistory->company_name)."' />&nbsp;&nbsp;<input type='button' onclick='popUpChooser(\"company_name\");' value='Select From List' /></td>");
echo("</tr>");
echo("<tr>");
echo("<td>Student Number:</td>");
echo("<td><input type='text' size='15' name='student_num' value='".$student_number."' />");
echo("&nbsp;<input type='button' name='student_select' onclick='popUpStudent(\"student_num\",\"myform\")' value='Select From List' />");
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td>");
echo("Term:&nbsp;&nbsp;");
echo("</td>");

$sql = ("
    SELECT term_id, term_name 
    FROM term 
    ORDER BY year_order
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$select_term = getNextTermYear();

echo("<td>");
echo("<select name='term_id'>");

while ($row = $result->FetchRow()) 
{
    if (!$addHistory->term_id && $row["term_id"] == $select_term["term"])
    {
        $selected = "selected='selected'";
    }
    elseif ($addHistory->term_id == $row["term_id"])
    {
        $selected = "selected='selected'";
    }

    echo("<option ".$selected." value='".$row['term_id']."'>".$row['term_name']."</option>");

    $selected = null;
}

echo("</select>");
echo("<select name='year'>");

for ($i=1;$i>=-1;$i--)
{
    $the_year = $select_term["year"] + $i;
    if (!$addHistory->year && $i == 0)
    {
        $selected = "selected='selected'";
    }
    elseif ($addHistory->year == $the_year)
    {
        $selected = "selected='selected'";
    }

    echo("<option ".$selected." value='".$the_year."'>".$the_year."</option>");

    $selected = null;
}
echo("</select>");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2'><hr /><div align='center'>");
echo("<input type='submit' name='submit' value='Add Record' /></div>");

// re-route to apporiate page, own job or returning job
if($select == "add_history")
{
    echo("<input type='hidden' name='select' value='add_history' />");
}
if($select == "returning_placement")
{
    echo("<input type='hidden' name='select' value='returning_placement' />");
}
echo("<input type='hidden' name='addHistory' value='".packObject(($addHistory))."' />");
echo("<input type='hidden' name='level' value='check_multiple_departments' />");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");

?>
