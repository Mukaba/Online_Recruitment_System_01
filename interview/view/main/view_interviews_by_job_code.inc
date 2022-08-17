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
 | Filename: view_interviews_by_job_code.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This if there are multiple jobs for a given job_code it         |
 | displays them for the user to select from                                    |
 +------------------------------------------------------------------------------+

*/
if ($job_id != ""){
	include ("interview/view/main/view_interview.inc");
}else{
	$sql = "SELECT j.job_id as job_id, j.job_code as job_code, e.company_name as company, MIN(i.int_date) as int_date
		FROM job_info as j, employer_company as e, interview_time as i
		WHERE j.job_id = i.job_id 
		AND j.employer_id = e.employer_id 
		AND j.job_code = '" . addslashes(stripslashes($job_code)) . "'
		AND j.department_id IN (" . $department_str . ") 
		GROUP BY job_id, job_code, company
		ORDER BY int_date";

	$result = $GLOBALS['dbh']->Execute($sql);

	$rows = $result->RecordCount();
	if ($rows == 0){
		echo ("Job was not found.");
	}elseif ($rows == 1){
		$row = $result->FetchRow();
		$job_id = $row["job_id"];
		include ("interview/view/main/view_interview.inc");
	}else{
		?>
		<h4>The following jobs match your search:</h4>

		<table border="1">
		  <tr>
		    <td class="rowgrey"><b class="white">First Date of Interviews</b></td>
		    <td class="rowgrey"><b class="white">Company</b></td>
		  </tr>
		  <?php
		  $i = 0;
		  while ($row = $result->FetchRow()){
			  ($i % 2 == 0) ? $class = "row0d" : $class = "row1d";
			  $i++;
			  $href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . $row["job_id"];
			  ?>
			  <tr>
			    <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["int_date"] == "" ) ? "&nbsp;" : $row["int_date"])?></a></td>
			    <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo (($row["company"] == "" ) ? "&nbsp;" : $row["company"])?></a></td>
			  </tr>
			  <?php
		  }
		?>
		</table>
		<?php
	}
}

