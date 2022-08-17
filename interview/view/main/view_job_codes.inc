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
 | Filename: view_job_codes.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user has entered a job code that   |
 | does not exactly match any of those in the database.  The user is shown a    |
 | list of job codes that are similar to the one they typed in, and they may    |
 | select any of the to see that job.                                           |
 |                                                                              |
 | NOTE: job_code and department_str must be made available in the script that  |
 | calls this file.                                                             |
 +------------------------------------------------------------------------------+

*/

/*
 Below has been added to ensure that we only pull out jobs that have interviews,
 rather than interviews AND job descriptions.
*/

$sql = ("
	SELECT a.job_id
	FROM interview AS a, job_info AS b
	WHERE b.department_id IN (" . $department_str . ")
	AND a.job_id=b.job_id
	");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
	$valid_ids .= ("'" . $row["job_id"] . "',");
}
$valid_ids = substr($valid_ids, 0, (strlen($valid_ids) - 1));
if ($valid_ids==NULL){
	$valid_ids="''";
}	
$sql = ("
	SELECT job_id  
	FROM job_info
	WHERE department_id IN (" . $department_str . ")
	AND job_id IN (" . $valid_ids . ")
	");
$result = $GLOBALS['dbh']->Execute($sql);

$count = $result->RecordCount();

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
$pages = ceil($count / $per_page_max);

?>
<form action='<?php echo $PHP_SELF?>' method='post'>
<input type="hidden" name="select" value="view" />
<input type="hidden" name="level1" value="job_code" />
<input type="hidden" name="level2" value="job_code_list" />
<?php
if (sizeof($departments_in_group)>1){
	?>
	<table border='0' cellspacing='0' cellpadding='4' class='row1'>
	  <tr>
	    <td>Department:</td>
	    <td><select name="department_id">
               <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach($departments_in_group as $department){
			if ($department == $department_id){
				echo "<option selected='selected' value='" . $department . "'>" . getDepartmentName($department) . "</option>\n";
			}else{
				echo "<option value='" . $department . "'>" . getDepartmentName($department) . "</option>\n";
			}
		}
		?>
		</select>
	     </td>
	   </tr>
	 </table>
	 <br />
	<?php
}
if ($valid_ids==NULL){
	$valid_ids="''";
}
$sql = ("
	SELECT job_code 
	FROM job_info 
	WHERE department_id IN (" . $department_str . ") 
	AND job_id IN (" . $valid_ids . ")
	ORDER BY job_code
	");
?>
<table border='0' cellspacing='0' cellpadding='4' class='row1'>
   <tr><td><?php echo("$count job" . (($count != 1) ? "s" : "") . " on $pages page" . (($pages > 1) ? "s" : "") . ";<br />"); ?></td>
   <td><input type="text" name="per_page_max" size="4" maxlength="4" value="<?php echo($per_page_max); ?>" /> jobs per page.</td></tr>
   <tr>
      <td>
         <select name="start_row">
<?php
$result = $GLOBALS['dbh']->Execute($sql);
for ($i = 0, $row_num_start = 0; $i < $pages; $i++, $row_num_start += $per_page_max)
{
   $row_num_end = $row_num_start + $per_page_max - 1;
   if ($row_num_end >= $count) $row_num_end = $count - 1;

   $result->Move($row_num_start);
   $job_code_row = $result->FetchRow();
   $job_code_start = $job_code_row[0];

   $result->Move($row_num_end);
   $job_code_row = $result->FetchRow();
   $job_code_end = $job_code_row[0];

   echo("<option value='$row_num_start'>" . $job_code_start . " to " . $job_code_end . "</option>\n");
}

?>
         </select>
      </td>
      <td align='right'><input type="submit" value="View" /></td>
   </tr>
</table>
</form>	
<?php

if ($start_row == '') { $start_row = 0; }

$first = $start_row + 1;
$end = $start_row + $per_page_max;
if ($end > $count) { $end = $count; }
//echo ($first . " to " . $end);

$result = $GLOBALS['dbh']->Execute("
   SELECT a.job_code, a.job_id, b.company_name
   FROM job_info AS a
   LEFT OUTER JOIN employer_company AS b ON a.employer_id=b.employer_id
   WHERE a.department_id IN (" . $department_str . ") 
   AND a.job_id IN (" . $valid_ids . ")
   ORDER BY a.job_code
   LIMIT $start_row, $per_page_max
");

                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Job Code (<?php echo ($first . "-" . $end); ?>)</b>&nbsp;</td>
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Company</b>&nbsp;</td>
                        </tr>
               <?php
      $rowclass = 0;
      while ($row = $result->FetchRow())
      {
         $job_id = $row["job_id"];
         $job_code = $row["job_code"];
         $company_name = $row["company_name"];
         $href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . urlencode($job_id) . "&amp;department_id=" . urlencode($department_id);

         echo("<tr>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".htmlentities($company_name)."</a>&nbsp;</td>\n");
         echo("</tr>\n");
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
