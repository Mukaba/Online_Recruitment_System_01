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
 | Filename: activate_main.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This page allows the user to select a job to view and           |
 | activate applications for.                                                   |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Activate</h3>
<table border="0" cellpadding="5" class="row1" cellspacing="0">
<form method="post" action="<?php echo $PHP_SELF?>">
<input type="hidden" name="page" value="activate" />
<input type="hidden" name="select" value="coverletter" />

<?php
  if (sizeof($departments_in_group) > 1){
	?>
	<tr>
	  <td>Department:</td>
	  <td><select name="department_id">
	  	<option value="0">All</option>
		<?php
		foreach($departments_in_group as $dep){
			echo "<option value='" . $dep ."'" . (($dep == $department_id) ? "selected='selected'" : "") . ">" . getDepartmentName($dep) . "</option>\n";
		}
		?>
		</select>
		<input type="submit" value="Go" />
	  </td>
	</tr>
<?php
  }
  ?>
  <tr>
    <td>Job Code:</td>
    <td><input type="text" name="job_code" size="30" value="<?php echo $job_code?>" /></td>
  </tr>
  <tr>
    <td colspan="2"><input type="submit" value="Activate" name="page" /></td>
  </tr>
</table>
</form>



<?php

$sql = "SELECT COUNT(c.job_id) as count, i.job_id as job_id, i.closing_date as closing_date, i.job_code, e.company_name
	FROM job_info as i left join employer_company as e ON e.employer_id = i.employer_id, coverletter as c, term as t
	WHERE i.status = ".CLOSED." AND c.job_id = i.job_id AND
		c.status_id IS NULL AND i.year = YEAR(CURRENT_DATE()) AND i.term_id=t.term_id 
	GROUP BY i.job_id";
$result = $GLOBALS['dbh']->Execute( $sql );

while ($row = $result->FetchRow()){
	
	//ensure and interview hasn't been scheduled
	$sql = "SELECT job_id 
		FROM interview
		WHERE job_id = '" . $row["job_id"] . "'";
	
	$result1 = $GLOBALS['dbh']->Execute($sql);
	
	if ($result1->RecordCount() <= 0){
		$job_ids[] = $row["job_id"];
		$job_codes[] = $row["job_code"];
		$company[]=$row["company_name"];
		$app[] = $row["count"];
		$closingdate[] = $row["closing_date"];
	}
}

if ( sizeof($job_ids) > 0 ){
	?>
	<br />
	Current Term's Jobs
	<table border="1">
	  <tr>
	    <td class="rowgrey"><b class="white">Job Code</b></td>
	    <td class="rowgrey"><b class="white">Company</b></td>
	    <td class="rowgrey"><b class="white">Number of<br />Applicants</b></td>
	    <td class="rowgrey"><b class="white">Date of Closing</b></td>
	  </tr>
	  <?php
	  $i = 0;
	  for($i=0;$i<sizeof($job_ids);$i++){
		(($i % 2 == 0) ? $class="row0d" : $class="row1d");
		$href = $PHP_SELF . "&amp;select=coverletter&amp;page=Activate&amp;job_id=" . $job_ids[$i];
		?>
		<tr>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($job_codes[$i]== "") ? "&nbsp" : $job_codes[$i])?></a></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($company[$i] == "") ? "&nbsp" : $company[$i])?></a></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($app[$i] == "") ? "&nbsp" : $app[$i])?></a></td>
		  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($closingdate[$i] == "") ? "&nbsp" : $closingdate[$i])?></a></td>
		</tr>
		<?php
	  }
	  ?>
	 </table>
<?php
}else{
	?>
	<table border="0"><tr><td>There are currently no jobs whose applications need to be activated.  All jobs<br />either have interviews scheduled, or there are no jobs in the database for the current term.</td></tr></table>
	<?php
}
?>
