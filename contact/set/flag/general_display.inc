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
 | Filename: general_display.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is used to show the user a list of contacts, along    |
 | with criteria to narrow down this list. This file also gives the user the    |
 | ability to set multiple flags at once for a large group of contacts.         |
 +------------------------------------------------------------------------------+

*/


?>

<script type="text/javascript" language="javascript">
<!--javascript

function checkAll()
{
    for (var i = 0; i < document.form2.elements.length; i++)
    {
        var e = document.form2.elements[i];
        if ((e.name != 'allbox') && (e.type == 'checkbox'))
        {
            e.checked = document.form2.allbox.checked;
        }
    }
}

function CheckCheckAll()
{
    var TotalBoxes = 0;
    var TotalOn = 0;

    for (var i=0; i < document.form2.elements.length; i++)
    {
        var e = document.form2.elements[i];
        if ((e.name != 'allbox') && (e.type=='checkbox'))
        {
            TotalBoxes++;
            if (e.checked)
            {
                TotalOn++;
            }
        }
    }

    if (TotalBoxes == TotalOn)
    {
        document.form2.allbox.checked = true;
    }
    else
    {
        document.form2.allbox.checked = false;
    }
}

function updateFlags()
{
    var j = 0;

    for (var i = 0; i < document.form2.elements.length; i++)
    {
        var e = document.form2.elements[i];
        if ((e.name != 'allbox') && (e.type == 'checkbox'))
        {
            var index = document.form2.contact_flag[document.form2.contact_flag.selectedIndex].value;

            var flag = flag_values[index];

            e.checked = flag[j];

            j++;
        }
    }
}

// This function checks if the user has made changes, and if so, confirms that they would like to leave the page without saving.
function checkForChanges()
{
    flagToCheck = document.form2.contact_flag[document.form2.contact_flag.selectedIndex].value;

    flagInitialValues = flag_values[flagToCheck];
    flagCurrentValues = Array();

    // Create the flagCurrentValues array
    var j = 0;
    for (var i = 0; i < document.form2.elements.length; i++)
    {
        var e = document.form2.elements[i];
        if ((e.name != 'allbox') && (e.type == 'checkbox'))
        {
            flagCurrentValues[j++] = e.checked;
        }
    }

    var changesMade = false;
    for (var i = 0; i < flag_values.length; i++)
    {
        if ((!flagInitialValues[i] && flagCurrentValues[i]) || (flagInitialValues[i] && !flagCurrentValues[i]))
        {
            // Changes have been made.  Confirm leaving the page.
            changesMade = true;
            var startRow = document.paging_form.start_row[document.paging_form.start_row.selectedIndex].value;
            window.open("contact/set/flag/confirm_paging.php?currentStartRow=" + startRow, "ConfirmPaging", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,width=350,height=175");
            break;
        }
    }

    if (!changesMade)
    {
        // They didn't make changes, so submit the form.
        var contact_flag = document.form2.contact_flag[document.form2.contact_flag.selectedIndex].value;
        document.paging_form.contact_flag.value = contact_flag;
        document.paging_form.submit();
    }
}

//-->

</script>

<?php

// These variables will help reduce the problem of too many contacts on a screen

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
if ($start_row == '') { $start_row = 0; }

// This array will keep all the contact flags. This helps reduce the number of calls to the database. 
$arrFlags = array();

$sql = ("
    SELECT DISTINCT dcs.flag_id, cf.flag_name
    FROM department_contact_flags AS dcs, contact_flags AS cf
    WHERE dcs.department_id='" . $auth->department . "'
    AND cf.flag_id=dcs.flag_id
    ORDER BY cf.flag_name 
    ");
$result = $GLOBALS['dbh']->Execute($sql);

// Copy SQL query into array.
$i=0;
while($flags_row = $result->FetchRow())
{
    $arrFlags[$i]['flag_id'] = $flags_row['flag_id'];
    $arrFlags[$i]['flag_name'] = $flags_row['flag_name'];
    $i++;
}
unset($i);

$specific_href = ($PHP_SELF . "&amp;select=set_contact_flag&amp;continue=show_specific&amp;level1=flags&amp;contact_id=");

// $checked_flags is a list of the flags that must be checked for the contacts returned.
if ($checked_flags && is_string($checked_flags))
{
    $checked_flags = unpackObject(($checked_flags));
}

if ($form_submitted)
{
    if (!$contact_flag)
    {
        // If they just came to this page, they won't have a contact flag chosen yet, so choose a default for them.
        $contact_flag = $arrFlags[0]["flag_id"];
    }

    if (!$contact_id_list_supplied && !$contact_id_list)
    {
        // We have not come here from advanced search
        $checked_flags = trim_array($checked_flags);

        // Pull the contacts out of the database, along with the flags requested.
        $sql = ("
            SELECT DISTINCT c.contact_id, c.last_name, c.first_name, ec.company_name, cfs.flag_id
            FROM contact AS c
            INNER JOIN contact_employer AS ce
            ON c.contact_id=ce.contact_id
            INNER JOIN employer_company AS ec
            ON ce.employer_id = ec.employer_id
            ");

        // These are flags that users are searching for.
        if (sizeof($checked_flags) > 0 && is_array($checked_flags))
        {
            for ($i=0; $i < sizeof($checked_flags); $i++)
            {
                $x = $i - 1;

                // In the first iteration, we have to join with the contact_employer table.
                if ($i==0)
                {
                    $sql .= ("
                        INNER JOIN contact_flags_set cfs".$i."
                        ON ce.contact_id = cfs".$i.".contact_id
                        ");
                }

                // Otherwise, join contact_flags_set with the last contact_flags_set table.
                else
                {
                    $sql .= ("
                        INNER JOIN contact_flags_set cfs".$i."
                        ON cfs".$x.".contact_id = cfs".$i.".contact_id
                        ");
                }
            }
        }

        $sql .= ("
            LEFT JOIN contact_flags_set AS cfs
            ON (cfs.contact_id=ce.contact_id AND cfs.flag_id='" . $contact_flag . "')
            LEFT JOIN department_contact_status AS dcs 
            ON dcs.contact_id=ce.contact_id
            WHERE ce.deleted_flag = '0'
            ");

        if (trim($first_name))
        {
            $sql .= ("
                AND c.first_name LIKE '" . addslashes(removeslashes(trim($first_name))) . "%'
                ");
        }

        if (trim($last_name))
        {
            $sql .= ("
                AND c.last_name LIKE '" . addslashes(removeslashes(trim($last_name))) . "%'
                ");
        }

        if (trim($company_name))
        {
            $sql .= ("
                AND ec.company_name LIKE '" . addslashes(removeslashes(trim($company_name))) . "%'
                ");
        }

        if (trim($contact_status_id))
        {
            $sql .= ("
                AND dcs.status_id='" . $contact_status_id . "' 
                AND dcs.department_id = '".$auth->department."'
                AND dcs.current_status = '1'
                ");
        }

        // Search for contacts with certain flags checked.
        if (sizeof($checked_flags) > 0 && is_array($checked_flags))
        {
            for ($i=0; $i < sizeof($checked_flags); $i++)
            {
                $sql .= ("
                    AND cfs".$i.".flag_id = '".$checked_flags[$i]."'
                    ");
            }
        }

        $sql .= ("
            ORDER BY c.last_name, c.first_name
            ");
    }

    // User came from advanced search or is ordering/paging the results. 
    else
    {
        $sql = ("
            SELECT DISTINCT c.contact_id, c.last_name, c.first_name, ec.company_name, cfs.flag_id
            FROM contact AS c
            INNER JOIN contact_employer AS ce
            ON c.contact_id=ce.contact_id
            INNER JOIN employer_company AS ec
            ON ce.employer_id = ec.employer_id
            LEFT JOIN contact_flags_set AS cfs
            ON (cfs.contact_id=ce.contact_id AND cfs.flag_id='" . $contact_flag . "')
            WHERE ce.deleted_flag = '0'
            AND ce.contact_id IN (" . $contact_id_list . ")
            ");

        if ($order)
        {
            $sql .= ("
                ORDER BY " . $order . "
                ");
        }
        else
        {
            $sql .= ("
                ORDER BY c.last_name, c.first_name
                ");
        }

    }

    $result = $GLOBALS['dbh']->Execute($sql);
    
    $i = 0;
    $temp_row = array();
    while($row = $result->FetchRow())
    {
        $temp_row[$i]['contact_id'] = $row['contact_id'];
        $temp_row[$i]['last_name'] = $row['last_name'];
        $temp_row[$i]['first_name'] = $row['first_name'];
        $temp_row[$i]['company_name'] = $row['company_name'];
        $temp_row[$i]['flag_id'] = $row['flag_id'];
        $i++;
    }
    unset($row);
    $row = $temp_row;
    unset($temp_row);
    $count = sizeof($row);

    // Set up the $count, $pages, and $end values for paging
    $pages = ceil($count / $per_page_max);

    $first = $start_row + 1;
    $end = $start_row + $per_page_max;
    if ($end > $count) 
    { 
        $end = $count; 
    }

    // Creating record id lists.
    // Reset the contact_id variable.
    unset($contact_ids);
    for ($i=0;$i<$count;$i++)
    {
        $contact_id_list .= $row[$i]['contact_id'].",";
        if ($i>=$start_row && $i<$end)
        {
            // contact_ids is an array of all contacts on the page. 
            $contact_ids[] = $row[$i]['contact_id'];
        }
    }

    // $contact_id_list is a comma delimited list of all the contact_ids
    $contact_id_list = substr($contact_id_list, 0, (strlen($contact_id_list) - 1));

    if ($count <= 0)
    {
        notify("There are no contacts in the database that match the criteria you have entered.");
    }
    else
    {
        /*
         We now have an array containing all of the contact_ids that we need to check,
         and an array containing all of the flags that can be set.  We need to form an
         array that holds the value of each flag for each contact id.
        */

        $str = ("var flag_values = new Array();");

        for ($i = 0; $i < sizeof($arrFlags); $i++)
        {
            $str2 .= ("flag_values[" . $arrFlags[$i]["flag_id"] . "] = Array(");

            for ($j = $start_row; $j < $end; $j++)
            {
                $flag_sql = ("
                    SELECT DISTINCT contact_id, flag_id
                    FROM contact_flags_set
                    WHERE contact_id='" . $row[$j]['contact_id'] . "'
                    AND flag_id='" . $arrFlags[$i]["flag_id"] . "'
                    ");
                $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                if (!$flag_result->RecordCount())
                {
                    $str2 .= ("'',");
                }
                else
                {
                    $str2 .= ("'checked',");
                }
            }

            // Clear out the trailing comma, and close the array call.

            $str2 = substr($str2, 0, (strlen($str2) - 1));
            $str2 .= (");");
        }

        ?>
        <script type="text/javascript" language="javascript">
        <!--javascript

        /*
         We now have the two strings, str and str2 that will be used to create our array
         holding all of the flag values for each contact id.  Let's parse them.
        */

        eval("<?php echo($str); ?>");
        eval("<?php echo($str2); ?>");

        //-->
        </script>
        <?php

        if ($count > 50) 
        {
            echo("<form method='post' name='paging_form' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>");
            echo("<input type='hidden' name='level1' value='flags' />");
            echo("<input type='hidden' name='form_submitted' value='true' />");

            echo("<input type='hidden' name='first_name' value='" . $first_name . "' />");
            echo("<input type='hidden' name='last_name' value='" . $last_name . "' />");
            echo("<input type='hidden' name='title' value='" . $title . "' />");
            echo("<input type='hidden' name='company_name' value='" . $company_name . "' />");
            echo("<input type='hidden' name='contact_status_id' value='" . $contact_status_id . "' />");
            echo("<input type='hidden' name='checked_flags' value='" . packObject(($checked_flags)) . "' />");
            echo("<input type='hidden' name='contact_id_list_supplied' value='true' />");
            echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
            echo("<input type='hidden' name='order' value='" . $order . "' />");
            echo("<input type='hidden' name='order_column_name' value='" . $order_column_name . "' />");
            // This is to aid our javascript methods.
            echo("<input type='hidden' name='contact_flag' value='' />");

            echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

            echo("<tr>");
            echo("<td>");
            echo($count . " contact" . (($count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
            echo(" contacts per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='start_row'>");

            for ($i = 0; $i < $pages; $i++)
            {
                $row_num_start = $i * $per_page_max;
                $row_num_end = $row_num_start + $per_page_max - 1;
                if ($row_num_end >= $count) 
                {
                    $row_num_end = $count - 1;
                }

                // What we display in the paging's drop down menu. 
                if ($order_column_name && $order)
                {
                    $contact_start = $row[$row_num_start][$order_column_name];
                }
                else
                {
                    $contact_start = $row[$row_num_start]['last_name'];
                }

                if (strlen($contact_start) > 15) 
                { 
                    $contact_start = substr($contact_start, 0, 13) . "..."; 
                }

                // What we display in the paging's drop down menu. 
                if ($order_column_name && $order)
                {
                    $contact_end = $row[$row_num_end][$order_column_name];
                }
                else
                {
                    $contact_end = $row[$row_num_end]['last_name'];
                }

                if (strlen($contact_end) > 15) 
                { 
                    $contact_end = substr($contact_end, 0, 13) . "..."; 
                }

                echo("<option value='" . $row_num_start . "' ");
                if ($start_row == $row_num_start)
                {
                        echo("selected='selected'");
                }
                echo(">" . $contact_start . " to " . $contact_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td align='right'>");
            //echo("<input type='submit' value='View' />");
            echo("<input type='button' value='View' onclick='checkForChanges();' />");
            echo("</td>");

            echo("</tr>");

            echo("<tr>");
                echo("<td align='center' colspan='2'>");
                echo("<hr />");
                echo("</td>");
            echo("</tr>");

            echo("</table>");
            echo("</form>");
            echo("<br />");
        }

        // These forms are used for ordering columns. 
        echo("<form name='form_order1' action='".$PHP_SELF."&amp;select=set_contact_flag&amp;level1=flags' method='post'>");
        echo("<input type='hidden' name='first_name' value='".$first_name."' />");
        echo("<input type='hidden' name='last_name' value='".$last_name."' />");
        echo("<input type='hidden' name='form_submitted' value='true' />");
        echo("<input type='hidden' name='company_name' value='".$company_name."' />");
        echo("<input type='hidden' name='per_page_max' value='".$per_page_max."' />");
        echo("<input type='hidden' name='start_row' value='".$start_row."' />");
        echo("<input type='hidden' name='contact_status_id' value='".$contact_status_id."' />");
        echo("<input type='hidden' name='contact_id_list_supplied' value='true' />");
        echo("<input type='hidden' name='contact_id_list' value='".$contact_id_list."' />");
        echo("<input type='hidden' name='contact_flag' value='".$contact_flag."' />");
        echo("<input type='hidden' name='order' value='c.last_name,c.first_name,ec.company_name' />");
        echo("<input type='hidden' name='order_column_name' value='last_name' />");
        echo("</form>");

        echo("<form name='form_order2' action='".$PHP_SELF."&amp;select=set_contact_flag&amp;level1=flags' method='post'>");
        echo("<input type='hidden' name='first_name' value='".$first_name."' />");
        echo("<input type='hidden' name='last_name' value='".$last_name."' />");
        echo("<input type='hidden' name='form_submitted' value='true' />");
        echo("<input type='hidden' name='company_name' value='".$company_name."' />");
        echo("<input type='hidden' name='per_page_max' value='".$per_page_max."' />");
        echo("<input type='hidden' name='start_row' value='".$start_row."' />");
        echo("<input type='hidden' name='contact_status_id' value='".$contact_status_id."' />");
        echo("<input type='hidden' name='contact_id_list_supplied' value='true' />");
        echo("<input type='hidden' name='contact_id_list' value='".$contact_id_list."' />");
        echo("<input type='hidden' name='contact_flag' value='".$contact_flag."' />");
        echo("<input type='hidden' name='order' value='ec.company_name,c.last_name,c.first_name' />");
        echo("<input type='hidden' name='order_column_name' value='company_name' />");
        echo("</form>");
        // End of ordering forms. 

        echo("<form method='post' name='form2' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>");
        echo("<input type='hidden' name='level1' value='flags' />");
        echo("<input type='hidden' name='form_submitted' value='true' />");

        echo("<input type='hidden' name='first_name' value='" . $first_name . "' />");
        echo("<input type='hidden' name='last_name' value='" . $last_name . "' />");
        echo("<input type='hidden' name='title' value='" . $title . "' />");
        echo("<input type='hidden' name='company_name' value='" . $company_name . "' />");
        echo("<input type='hidden' name='contact_status_id' value='" . $contact_status_id . "' />");
        echo("<input type='hidden' name='checked_flags' value='" . packObject(($checked_flags)) . "' />");
        echo("<input type='hidden' name='contact_id_list_supplied' value='" . $contact_id_list_supplied . "' />");
        // This is to aid our javascript functions.
        echo("<input type='hidden' name='continue' value='' />");
        // A list of ALL the contact_ids
        echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
        // A list of all contact ids on the page. 
        echo("<input type='hidden' name='contact_ids' value='" . packObject(($contact_ids)) . "' />");
        echo("<input type='hidden' name='start_row' value='" . $start_row . "' />");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
        echo("<tr>");
        echo("<td>");
        echo("<table border='0' cellpadding='2'>");
        echo("<tr>");

        // These links will call our ordering forms declared above. 
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;<a class='orderable' href='javascript:document.form_order1.submit()'><b class='white'>Contact Name</b></a>&nbsp;&nbsp;</td>");
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;<a class='orderable' href='javascript:document.form_order2.submit()'><b class='white'>Company Name</b></a>&nbsp;&nbsp;</td>");
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;");

        echo("<select name='contact_flag' onchange='updateFlags(); CheckCheckAll();'>");
        for ($i=0;$i<sizeof($arrFlags);$i++)
        {
            echo("<option value='" . $arrFlags[$i]["flag_id"] . "' ");
            if ($contact_flag == $arrFlags[$i]["flag_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $arrFlags[$i]["flag_name"] . "</option>");
        }
        echo("</select>");

        echo("&nbsp;&nbsp;</td>");
        echo("</tr>");

        $rowclass = 0;

        // Display contacts found in search. 
        for($i=$start_row;$i<$end;$i++)
        {
            echo("<tr>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<a class='blue' href='" . $specific_href . $row[$i]["contact_id"] . "'>" . $row[$i]["last_name"].", ".$row[$i]["first_name"] . "</a>&nbsp;</td>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<a class='blue' href='" . $specific_href . $row[$i]["contact_id"] . "'>" . $row[$i]["company_name"] . "</a>&nbsp;</td>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<input type='checkbox' class='" . (($rowclass) ? "row0d" : "row1d") . "' name='flag_on[]' value='" . $row[$i]["contact_id"] . "' onclick='CheckCheckAll()' ");
            if ($row[$i]["flag_id"])
            {
                echo("checked='checked'");
            }
            else
            {
                // Set a variable so we know that Select/Deselect All doesn't start off checked.
                $checkCheckAllOff = true;
            }
            echo(" /></td>");
            echo("</tr>");

            $rowclass = (($rowclass) ? 0 : 1);
        }

        echo("<tr>");
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='2' align='center'>");
        // A list of all contact ids on this page. 
        if ($count > $per_page_max) 
        {
            echo("<input type='submit' name='continue' value='Set Flag and Save for All Contacts on All Pages' />");
            echo("&nbsp;");
            echo("<input type='submit' name='continue' value='Unset Flag and Save for All Contacts on All Pages' />");
            echo("<br />");
        }
        echo("<input type='submit' name='continue' value='Save Changes' />");
        echo("</td>");
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
        echo("<b class='black'>Select/Deselect Flag<br />for All Contacts Shown: </b>");
        echo("<input type='checkbox' class='" . (($rowclass) ? "row0d" : "row1d") . "' name='allbox' onclick='checkAll()' " . (!$checkCheckAllOff ? "checked='checked'" : "") . " />");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("<table border='0'>");
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("</tr>");
        echo("</table>");
        echo("</form>");
    }
}

echo("<form method='post' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>");
echo("<input type='hidden' name='level1' value='flags' />");
echo("<input type='hidden' name='form_submitted' value='true' />");

echo("<table border='0' cellpadding='2' cellspacing='0'>");
echo("<tr align='center'>");
echo("<td class='row1'>");
echo("<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
echo("<tr align='center'>");
echo("<td colspan='2' align='center'>");
echo("<b class='black'>Show contacts that match the criteria below:<br />(Leave any fields blank that you do not wished to be used in the search)</b>");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2'><hr width='50%' size='1' /></td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("First name starts with: ");
echo("<input type='text' name='first_name' size='10' value='" . $first_name . "' />");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("Last name starts with: ");
echo("<input type='text' name='last_name' size='10' value='" . $last_name . "' />");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("Company starts with: ");
echo("<input type='text' name='company_name' size='10' value='" . $company_name . "' />");
echo("</td>");
echo("</tr>");

$type_sql = ("
    SELECT status_id, status_name
    FROM employer_info_statuses
    ORDER BY level_of_activity
    ");
$type_result = $GLOBALS['dbh']->Execute($type_sql);

echo("<tr>");
echo("<td colspan='2' align='center'>");
echo("Status of Contact: ");
echo("<select name='contact_status_id'>");
echo("<option value='' ");
if (!$contact_status_id)
{
    echo("selected='selected'");
}
echo(">&nbsp;</option>");
while ($type_row = $type_result->FetchRow())
{
    echo("<option value='" . $type_row["status_id"] . "' ");
    if ($contact_status_id == $type_row["status_id"])
    {
        echo("selected='selected'");
    }
    echo(">" . $type_row["status_name"] . "</option>");
}
echo("</select>");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

if (sizeof($arrFlags) > 0 && is_array($arrFlags))
{
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("Contacts with these flags checked:");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table align='center' cellpadding='3' cellspacing='3'>");
    echo("<tr align='center'>");
    echo("<td align='center' class='row2'>");
    echo("<table width='100%' class='row2'>");

    $newrow = 0;

    for ($i=0;$i<sizeof($arrFlags);$i++)
    {		
        if (!$newrow)
        {
            echo("<tr>");
        }

        echo("<td>");
        echo("<input type='checkbox' name='checked_flags[]' value='" . $arrFlags[$i]["flag_id"] . "' ");
        if (is_array($checked_flags) && in_array($arrFlags[$i]["flag_id"], $checked_flags))
        {
            echo("checked='checked'");
        }
        echo(" />");
        echo($arrFlags[$i]["flag_name"]);
        echo("</td>");

        if ($newrow)
        {
            echo("</tr>");
        }

        $newrow = (($newrow) ? 0 : 1);
    }

    if ($newrow)
    {
        echo("<td>&nbsp;</td>");
        echo("</tr>");
    }

    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");

}

echo("<tr>");
echo("<td colspan='2' align='center'><hr />");
echo("<input type='submit' value='Go' />");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");
?>
