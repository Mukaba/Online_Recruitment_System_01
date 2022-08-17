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
 | Filename: view_applications_main.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: Here the user can enter a job code or select one from a list    |
 | of the job that they wish to view the active applications for.               |
 +------------------------------------------------------------------------------+

*/
?>

<h3>View Applications</h3>

<form method="post" action="<?php echo $PHP_SELF?>">
<input type="hidden" name="select" value="coverletter"/>
<input type="hidden" name="page" value="application" />
<table border="0" cellpadding="5" class="row1" cellspacing="0">
<?php
if (sizeof($departments_in_group) > 1){
	?>
  <tr>
    <td>Department:</td>
    <td><select name="department_id">
        <option value='0'>All</option>
    <?php
	foreach($departments_in_group as $dep){
		echo "<option value='$dep' " . (($dep == $department_id) ? "selected='selected'" : "") . ">" . getDepartmentName($dep) . "</option>\n";
	}
    ?>
        </select>
	<!-- INPUT type="submit" value="Go" -->
    </td>
  </tr>
<?php
}
?>
  <tr>
    <td>Job Code:</td>
    <td><input type="text" name="job_code" size="30" /></td>
  </tr>
  <tr>
    <td colspan="2"><input type="submit" value="View Applications" name="page" /></td>
  </tr>
</table>
</form>


<?php


$sql = "SELECT count(c.job_id) as count, c.job_id, i.job_code, e.company_name, t.term_name, i.year
	FROM coverletter as c, job_info as i left join employer_company as e ON e.employer_id =  i.employer_id, term as t
	WHERE c.job_id = i.job_id AND t.term_id = i.term_id AND c.status_id = " . ACTIVE . "
	GROUP BY c.job_id";
		//AND YEAR(CURRENT_DATE()) = i.year AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date GROUP BY c.job_id";
		//AND i.department_id IN (" . $department_in_str . ") AND YEAR(CURRENT_DATE()) = i.year AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date GROUP BY c.job_id";


$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() > 0){
	?>
	<table border="0" cellpadding="0" cellspacing="0"><tr><td>Current term jobs, in selected department, with active cover letters:</td></tr></table>
	<table border="1">
	  <tr>
	    <td class="rowgrey"><b class="white">Job Code</b></td>
	    <td class="rowgrey"><b class="white">Company</b></td>
	    <td class="rowgrey"><b class="white">Term</b></td>
	    <td class="rowgrey"><b class="white">Year</b></td>
	    <td class="rowgrey"><b class="white">Number Applicants</b></td>
	  </tr>
	<?php
	$i = 0;
	while($row = $result->FetchRow()){
		($i % 2==0) ? $class="row0d" : $class="row1d";
		
		$href= $PHP_SELF . "&amp;select=coverletter&amp;page=View+Applications&amp;job_id=" .  $row["job_id"];
		?>
		<tr>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["job_code"] == "") ? "&nbsp;" : $row["job_code"])?></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["company_name"] == "") ? "&nbsp;" : $row["company_name"])?></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["term_name"] == "") ? "&nbsp;" : $row["term_name"])?></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["year"] == "") ? "&nbsp;" : $row["year"])?></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["count"] == "") ? "&nbsp;" : $row["count"])?></td>
		</tr>
		<?php
	}
	?>
	</table>
	<?php
}else{
	?>
	<table border="0" cellpadding="0" cellspacing="0"><tr><td>Currently there are no active cover letters for this term in the department you have selected.</td></tr></table>
	<?php
}
?>
