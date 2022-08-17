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
 | Filename: view_main.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the job codes that have the employer     |
 | loged in assocated with them.  If an interview has been scheduled the job    |
 | code is a link to the interview schedule.                                    |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Interview Schedules</h3>
<?php
$sql = "SELECT j.job_code, j.job_id
	FROM job_info as j, term as t, contact_job as c 
	WHERE j.interview_term_id = t.term_id and j.job_id = c.job_id AND 
		c.contact_id = '"  . $auth->contact_id. "' AND j.interview_year = YEAR(CURRENT_DATE())
		AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date";


$result = $GLOBALS['dbh']->Execute($sql);

$job_ids = array();
while ($row = $result->FetchRow()){
	$sql = "SELECT job_id FROM interview 
		WHERE job_id = '" . $row["job_id"] . "'";
	$result1 = $GLOBALS['dbh']->Execute($sql);
	if ($result1->RecordCount() <= 0){
		$job_ids[] = $row["job_id"];
		$job_codes[] = $row["job_code"];
		$msg[] = "No interview has been scheduled this time.";
		$link[] = 0;
	}else{
		$job_ids[] = $row["job_id"];
		$job_codes[] = $row["job_code"];
		$msg[] = "";
		$link[] = 1;
	}
}

if (count($job_ids) == 0){
	echo ("You have no jobs for this term in the database.");
}else{
	?>
	<table border="1">
	  <tr>
	    <td class="rowgrey"><b class="white">Job Code</b></td>
	    <td class="rowgrey"><b class="white">Information</b></td>
	  </tr>

	  <?php
	  for($i=0;$i<count($job_ids);$i++){
		  (($i%2==0) ? $class="row0d" : $class="row1d");
		  ?>
		  <tr>
		    <td class="<?php echo $class?>"><?php echo (($link[$i]) ? "<a class='blue' href='" . $PHP_SELF . "&amp;select=view&amp;job_id=" . addslashes($job_ids[$i]) . "&amp;level1=view" . "'>" . $job_codes[$i] . "</a>" : $job_codes[$i])?></td>
		    <td class="<?php echo $class?>"><?php echo (($msg[$i]!="") ? $msg[$i] : "&nbsp")?></td>
		  </tr>
		  <?php
	  }
	  ?>
	</table>
<?php
}
?>
