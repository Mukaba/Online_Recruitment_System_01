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
 | Description: This file will list all the jobs the employer who is logged     |
 | in is associated with.  If applications has been activated for the job the   |
 | employer will be able to click on the job and see the applications.          |
 +------------------------------------------------------------------------------+

*/

?>

<form action="<?php echo $PHP_SELF?>" method="post">
<input type="hidden" name="select" value="coverletter" />
<h3>View Applicants</h3>


<table border="0" cellpadding="5" cellspacing="0" class='backgrey'>
  <tr>
    <td><h4>Application Information:</h4>
    	The following list contains all the coop jobs you have submitted this term.  To view the list of students who have applied to your job and their applications 
	simply click on the job code.  If you cannot click on the job code then the applications have no yet been activated or will not be viewable through this system.
	<br />
    </td>
  </tr>
</table>
<br />
<br />
<?php

if ($userlevel == EMPLOYER){
	$sql = "SELECT j.job_id, j.job_code, j.department_id
		FROM contact_job as c, job_info as j, term as t
		WHERE j.term_id = t.term_id AND j.job_id = c.job_id AND c.contact_id = '" . $auth->contact_id . "'
			AND j.year = YEAR(CURRENT_DATE())";
			// AND j.year = YEAR(CURRENT_DATE()) AND t.term_id = j.term_id AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date";
}elseif($userlevel == HREMPLOYER){
	$sql = "SELECT j.job_id, j.job_code, j.department_id
		FROM job_info as j, term as t, employer_login as e
		WHERE e.contact_id = '" . $auth->contact_id . "' AND e.employer_id = j.employer_id
			AND j.year = YEAR(CURRENT_DATE())";
			// AND t.term_id =j.term_id AND MONTH(CURRENT_DATE()) BETWEEN t.start_date AND t.end_date";
}

$result = $GLOBALS['dbh']->Execute($sql);
$i=0;
if ($result->RecordCount() == 0){
	echo "You currently do not have any jobs in the system with viewable applications.";
}else{
	?>
	<table border="1">
	  <tr>
	    <td class='rowgrey'><b class='white'>Job Code</b></td>
	    <td class='rowgrey'><b class='white'>Information</b></td>
	  </tr>
	<?php
	while ($row=$result->FetchRow()){
		$class = (($i % 2==0) ? "row0d" : "row1d");
		$i++;
		$link = 1;
		$msg = "";
	
		//ensure that at least one coverletter has been activated
		$sql = "SELECT student_number
			FROM coverletter
			WHERE job_id = '" . $row["job_id"] . "' AND
				status_id = '" . ACTIVE . "'";
		$result1 = $GLOBALS['dbh']->Execute($sql);
	
		if ($result1->RecordCount() <= 0){
			$link = 0;
			$msg = "Applications for this job have not been sent yet.";
		}else{
		
			//ensure the department has the resume module
			$sql = "SELECT module_id
				FROM department_module
				WHERE department_id = '" . $row["department_id"] . "' AND 
					module_id = 6";
			$result1a = $GLOBALS['dbh']->Execute($sql);
			if ($result1a->RecordCount() <= 0){
				$link = 0;
				$msg = "Applications for this job will not be viewable electronically through this system";
			}
	
			//check to ensure they are recieving electronic (not paper applications)
		}
		?>
		<tr>
		  <td class="<?php echo $class?>"><?php echo (($link) ? "<a class='blue' href='" . $PHP_SELF . "&amp;select=coverletter&amp;page=1&amp;job_id=" . $row["job_id"] . "'>" : "" ); echo $row["job_code"]; echo (($link)? "</a>" : "");?></td>
		  <td class="<?php echo $class?>"><?php echo (($msg == "") ? "&nbsp" : $msg)?></td>
		</tr>
		<?php
	}
}
?>

</table>
</form>
