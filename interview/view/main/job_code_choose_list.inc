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
 | Filename: job_code_choose_list.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user is trying to view jobs for    |
 | a job code.  If the user does not type an exact match for a job code, then   |
 | this file is called, showing them all of the job codes that were close       |
 | matches with what they typed, and allowing them to choose from the list.     |
 |                                                                              |
 | NOTE: job_code and department_str must be available by the script that       |
 | calls this file.                                                             |
 +------------------------------------------------------------------------------+

*/

$sql = ("
   SELECT ji.job_code, ji.job_id, ec.company_name
   FROM job_info AS ji
   INNER JOIN interview i
   ON ji.job_id = i.job_id
   LEFT OUTER JOIN employer_company AS ec 
   ON ji.employer_id=ec.employer_id
   WHERE ji.department_id IN (" . $department_str . ") 
   AND ji.job_code LIKE '" . addslashes($job_code) . "%'
   ORDER BY ji.job_code
   ");
$result = $GLOBALS['dbh']->Execute($sql);

$count = $result->RecordCount();

if ($count > 0) {

	if ($per_page_max == "") { $per_page_max = 50; }
	if ($per_page_max < 5) { $per_page_max = 5; }
	$pages = ceil($count / $per_page_max);
	
	?>
	<form action='<?php echo $PHP_SELF?>' method='post'>
	<input type="hidden" name="select" value="view" />
	<input type="hidden" name="level1" value="job_code" />
	<input type="hidden" name="level2" value="close_match" />
	<input type='hidden' name='job_code' value='<?php echo($job_code); ?>' />
	
	<?php
	$msg = ("The job code you entered does not match exactly with any of those currently");
	$msg .= (" in the database.  Please choose the correct company from those listed below.");
	instruct($msg);
	?>
<br />	
<?php
	if ($count > $per_page_max) {
?>
	<table border='0' class='row1' cellspacing='0' cellpadding='4'>
	   <tr><td><?php echo("$count job" . (($count != 1) ? "s" : "") . " on $pages page" . (($pages > 1) ? "s" : "") . ";<br />"); ?></td>
	   <td><input type="text" name="per_page_max" size="4" maxlength="4" value="<?php echo($per_page_max); ?>" /> jobs per page.</td></tr>
	   <tr>
	      <td>
	         <select name="start_row">
	<?php
	
	for ($i = 0, $row_num_start = 0; $i < $pages; $i++, $row_num_start += $per_page_max)
	{
	   $row_num_end = $row_num_start + $per_page_max - 1;
	   if ($row_num_end >= $count) $row_num_end = $count - 1;
	
       $result->Move($row_num_start);
       $job_code_row = $result->FetchRow();
       $job_code_start = $job_code_row['job_code']; 

       $result->Move($row_num_end);
       $job_code_row = $result->FetchRow();
       $job_code_end = $job_code_row['job_code']; 

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
}
	if ($start_row == '') { $start_row = 0; }

	$first = $start_row + 1;
	$end = $start_row + $per_page_max;
	if ($end > $count) { $end = $count; }
	//echo ($first . " to " . $end);

    ?>
      <table cellspacing="0" cellpadding="0" border="1"><tr><td>
         <table border="0" cellpadding="2">
            <tr>  
               <td class="rowgrey" align='center'>&nbsp;<b class="white">Job Code (<?php echo ($first . "-" . $end); ?>)</b>&nbsp;</td>
               <td class="rowgrey" align='center'>&nbsp;<b class="white">Company</b>&nbsp;</td>
            </tr>
   <?php

      $rowclass = 0;
      $result->Move($start_row);

      while (($row = $result->FetchRow()) && ($rowclass < $per_page_max)) 
      {
         $job_id = $row["job_id"];
         $job_code = $row["job_code"];
         $company_name = $row["company_name"];
         $href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . urlencode($job_id) . "&amp;department_id=" . urlencode($department_id);

         echo("<tr>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");
         echo("</tr>\n");
         $rowclass++;
      }

      ?>
          </table>
       </td></tr></table>
<?php 
} else {

	notify("No interviews for the selected job code and department.");
	include("interview/view/list/view_by_job_code_main.inc");
}

?>
