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
 | Filename: company_choose_list.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user is trying to view jobs for    |
 | a company.  If the user does not type an exact match for a company, then     |
 | this file is called, showing them all of the companies that were close       |
 | matches with what they typed, and allowing them to choose from the list.     |
 |                                                                              |
 | NOTE: company_name and department_str must be made available in the script   |
 | that includes this file.                                                     |
 +------------------------------------------------------------------------------+

*/

if ($department_id == ""){
	$department_id = $auth->department;
}

$result = $GLOBALS['dbh']->Execute("
	SELECT count(distinct(e.employer_id)) AS companies 
	FROM employer_company as e, job_info as j 
	WHERE e.employer_id = j.employer_id 
	AND j.department_id IN (" . $department_str . ")
	AND e.company_name LIKE '" . $company_name . "%'
	");

$row = $result->FetchRow();
$count = $row["companies"];

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
$pages = ceil($count / $per_page_max);

?>
<form action='<?php echo $PHP_SELF?>' method='post'>
<input type="hidden" name="select" value="view" />
<input type="hidden" name="level1" value="company" />
<input type="hidden" name="level2" value="close_match" />
<input type='hidden' name='company_name' value='<?php echo($company_name); ?>' />
<input type="hidden" name="past" value="<?php echo($past); ?>" />

<?php
$msg =("The company name you entered did not match exactly with any of those currently");
$msg .= (" in the database.  Please choose the correct company from those listed below.");
instruct($msg);
?>
<br />
<?php 
if ($count > $per_page_max) {
?>
<table class='row1' border='0' cellspacing='0' cellpadding='4'>
   <tr><td><?php echo("$count compan" . (($count != 1) ? "ies" : "y") . " on $pages page" . (($pages > 1) ? "s" : "") .";<br />"); ?></td>
   <td><input type="text" name="per_page_max" size="4" maxlength="4" value="<?php echo($per_page_max); ?>" /> companies per page.</td></tr>
   <tr>
      <td>
         <select name="start_row">
<?php

$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT(e.employer_id)
			FROM employer_company as e, job_info as j 
			WHERE j.employer_id = e.employer_id 
			AND j.department_id IN (" . $department_str . ") 
			AND e.company_name LIKE '" . $company_name . "%'
			ORDER BY e.company_name
			");
for ($i = 0; $i < $pages; $i++)
{
    $row_num_start = $i * $per_page_max;
    $row_num_end = $row_num_start + $per_page_max - 1;
    if ($row_num_end >= $count) $row_num_end = $count - 1;

    $result->Move($row_num_start);
    $company_row = $result->FetchRow();
    $company_start = getCompanyName($company_row[0]);
    if (strlen($company_start) > 15) { $company_start = substr($company_start, 0, 13) . "..."; }

    $result->Move($row_num_end);
    $company_row = $result->FetchRow();
    $company_end = getCompanyName($company_row[0]);
    if (strlen($company_end) > 15) { $company_end = substr($company_end, 0, 13) . "..."; }

    echo("<option value='$row_num_start'>" . $company_start . " to " . $company_end . "</option>\n");
}
?>
         </select>
      </td>
      <td align='right'><input type="submit" value="View" /></td>
   </tr>
</table>
</form>	
<?php
}

if ($start_row == '') { $start_row = 0; }

$first = $start_row + 1;
$end = $start_row + $per_page_max;
if ($end > $count) { $end = $count; }

$result = $GLOBALS['dbh']->Execute("
   SELECT DISTINCT(e.employer_id), e.company_name 
   FROM employer_company as e, job_info as j
   WHERE j.employer_id = e.employer_id 
   AND j.department_id IN (" . $department_str . ") 
   AND e.company_name LIKE '" . $company_name . "%'
   GROUP BY e.company_name
   ORDER BY e.company_name
   LIMIT $start_row, $per_page_max");
                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Company (<?php echo ($first . "-" . $end); ?>)</b>&nbsp;</td>
                        </tr>
               <?php
      $rowclass = 0;
      while ($row = $result->FetchRow())
      {
         $employer_id = $row["employer_id"];
         $company_name = $row["company_name"];
         $href = $PHP_SELF . "&amp;select=view&amp;level1=company&amp;level2=company_name_or_employer_id&amp;employer_id=" . urlencode($employer_id) . "&amp;past=" . urlencode($past) . "&amp;department_id=" . urlencode($department_id);

         echo("<tr>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>");
         echo("</tr>\n");
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php

    

?>
