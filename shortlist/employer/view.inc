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
 | Filename: view.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*
 Filename: view.inc
 Description: This page lists all the jobs that are associated with the employer logged on.  If the applications 
 have been activated they can shortlisted click on the job to shortlist the students.
 :NOTE: This file is now deprecated and should be removed from the next release.
*/
?>

<h3>Shortlist Students</h3>

<?
/*$sql = "SELECT j.job_code, j.job_id
	FROM job_info as j, term as t, contact_job as c 
	WHERE j.term_id = t.term_id and j.job_id = c.job_id AND c.contact_id = '" .$auth->contact_id. "' 
		AND j.year = YEAR(CURRENT_DATE())"; */
		// AND j.year = YEAR(CURRENT_DATE()) AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date";
$sql = "select j.job_code, j.job_id from job_info as j, term as t, contact_employer as ce where 
        j.term_id=t.term_id and j.employer_id=ce.employer_id and ce.contact_id=\"".$auth->contact_id."\"";
//echo $sql;

$result = $GLOBALS['dbh']->Execute($sql);

$job_ids = array();
while ($row = $result->FetchRow()){
//	$sql = "SELECT job_id FROM job_info
//		WHERE job_id = '".$row["job_id"]."' AND status = '".ACTIVE."'";
	$sql = "SELECT job_id FROM job_info as j, job_status as js
		WHERE job_id = '".$row["job_id"]."' AND j.status = js.status_id and js.status_name=\"Closed\"";
        echo $sql;
	$result1 = $GLOBALS['dbh']->Execute($sql);
	if ($result1->RecordCount() <= 0){
		$job_ids[] = $row["job_id"];
		$job_codes[] = $row["job_code"];
		$msg[] = "Applications not yet sent";
		$link[] = 0;
	}else{
		$job_ids[] = $row["job_id"];
		$job_codes[] = $row["job_code"];
		$msg[] = "";
		$link[] = 1;
	}
}

if (count($job_ids) == 0){
	?>
	You have no jobs for this term in the database.
	<?
}else{
	?>
	<table border="1">
	  <tr>
	    <td class="rowgrey"><b class="white">Job Code</b></td>
	    <td class="rowgrey"><b class="white">Information</b></td>
	  </tr>

	  <?
	  for($i=0;$i<count($job_ids);$i++){
		  (($i%2==0) ? $class="row0d" : $class="row1d");
		  ?>
		  <tr>
		    <td class="<?php echo $class?>"><?php echo (($link[$i]) ? "<a class='blue' href='".$PHP_SELF."&select=shortlist&job_id=".addslashes($job_ids[$i])."&level=1"."'>".$job_codes[$i]."</a>" : $job_codes[$i])?></td>
		    <td class="<?php echo $class?>"><?php echo (($msg[$i]!="") ? $msg[$i] : "&nbsp")?></td>
		  </tr>
		  <?
	  }
	  ?>
	</table>
<?
}
?>
