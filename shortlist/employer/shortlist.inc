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
 | Filename: shortlist.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*
 Filename: shortlist.inc
 Description: If a shortlist has not yet been set up for this job this page will have a list of all the students who applied
 to the job, and allow the employer to select which they want to interview.  If a shortlist has been set up
 this page will make the shortlist avaliable to the employer.
 :NOTE: This file is now deprecated and should be removed from the next release.
*/


if ($job_id == "" || !isValidJobID($job_id)){
	error ("Invalid Job Id");
	include ("shortlist/employer/view.inc");
}else{
	$sql = "SELECT j.job_id
		FROM job_info as j, contact_employer as ce
		where j.job_id=\"".$job_id."\" and j.employer_id=ce.employer_id and ce.contact_id=\"".$auth->contact_id.
                "\"";
//		WHERE j.job_id = '". $job_id . "' AND contact_id = '" . $auth->contact_id . "'";
//	echo $sql;
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result || $result->RecordCount() <= 0){
		error ("Invalid Job ID");
		include("shortlist/employer/view.inc");
	}else{
		$result = $GLOBALS['dbh']->Execute("SELECT job_code FROM job_info WHERE job_id = '" .$job_id . "'");	
		$row = $result->FetchRow();
		$job_code = $row["job_code"];
		
		
		$sql = "SELECT student_number FROM students_shortlisted WHERE job_id = '" . $job_id . "'";
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount() > 0){
			?>	
			<h3>Shortlist Students</h3>
			<h4>Job Code: <?php echo $job_code ?></h4>
			<?
/*			$result1a = $GLOBALS['dbh']->Execute("SELECT first_name, last_name, email
			  			FROM contact as c, interview as i
						WHERE c.contact_id = i.contact AND i.job_id =$job_id");
			if($result1a->RecordCount()<=0){
				$result1a = $GLOBALS['dbh']->Execute("SELECT first_name, last_name, email
							  FROM contact as c, job_info as i, contact_internal as a
							  WHERE i.job_id = $job_id AND 
							  a.contact_id = c.contact_id AND a.department_id = i.department_id");
			}
*/
			$result1a = $GLOBALS['dbh']->Execute ("select first_name, last_name, email from contact as c, job_info as j ".
						 "where j.internal_contact_id=c.contact_id and j.job_id=$job_id");

			$row1a = $result1a->FetchRow();
			?> 
			To modify this list please contact <?php echo $row1a["first_name"] . " " . $row1a["last_name"]?> at <a class="blue" href="mailto:<?php echo $row1a["email"]?>"><?php echo $row1a["email"]?></a>

			
			<br /><br />The following students have been shortlisted for this job:
			
			<table border="0" cellspacing="1" cellpadding="4">
			  <tr>
			    <td class="rowgrey"><b class="white">Student</b></td>
			  </tr>
			  <?php 
			  $in_str = "";
			  $i=0;
			  while ($row = $result->FetchRow()){
				  $in_str .= "'" . $row["student_number"] . "',";
				  (($i%2==0) ? $class="row0d" : $class="row1d");
				  $i++;
				  ?>
				  <tr>
				    <td class="<?php echo $class?>"><?php echo getStudentName($row["student_number"])?></td>
				  </tr>
				  <?
			  }
			  $in_str = substr($in_str, 0, strlen($in_str) - 1);
			  ?>
			  </table>
			  <?

			  $sql = "select student_number from students_shortlisted where job_id=\"$job_id\" and ".
				   "student_number not IN($in_str)";

/*
			  $sql = "SELECT student_number FROM coverletter WHERE job_id = '" . $job_id . "' AND 
			  		student_number NOT IN (" . $in_str . ") AND status_id = " . ACTIVE;
*/
			  $result = $GLOBALS['dbh']->Execute($sql);
			  if ($result->RecordCount() > 0){
				  ?>
				  <br /><br />You have elected not to shortlist the following students:
				  <table border="1">
				    <tr>
				      <td class="rowgrey"><b class="white">Student</b></td>
				    </tr>
				    <?
				    while ($row = $result->FetchRow()){
					    (($i%2==0) ? $class="row0d" : $class="row1d");
					    $i++;
					    ?>
					    <tr>
					      <td class="<?php echo $class?>"><?php echo getStudentName($row["student_number"])?></td>
					    </tr>
					    <?
				    }
				    ?>
				    </table>
				    <?
			  }
		}else{
			$result = $GLOBALS['dbh']->Execute("SELECT job_code FROM job_info WHERE job_id = '" . $job_id . "'");
		
			$sql = "SELECT s.student_number, s.first_name, s.last_name, s.middle_name
				FROM student as s, job_info as j, applications as a 
				WHERE s.student_number = a.student_number and a.job_id=j.job_id and 
                                        j.job_id=\"$job_id\" and j.status=\"CLOSED\"
				ORDER BY last_name, first_name, middle_name, student_number";
			$result = $GLOBALS['dbh']->Execute($sql);
			?>
			<h3>Short List Students</h3>
			<h4>Job Code: <?php echo $job_code ?></h4>
			<?
			if($result->RecordCount() <= 0){
				echo "No students to shortlist for this job";
			}else{
				?>
				
				<form action="<?php echo $PHP_SELF?>" method="post">
				<input type="hidden" name="select" value="shortlist" />
				<input type="hidden" name="level" value="2" />
				<input type="hidden" name="job_id" value="<?php echo $job_id?>" />
				<table border="1">
				  <tr>
				    <td class="rowgrey">&nbsp;</td>
			    <td class="rowgrey"><b class="white">Student Name</b></td>
				  </tr>
				  <?
				  $i=0;
				  $row = $result->FetchRow();
				  
				  $last = $row["last_name"];
				  $first = $row["first_name"];
				  $middle = $row["middle_name"];
				  $number = $row["student_number"];
	
					  while ($row = $result->FetchRow()){
					  (($i%2==0) ? $class="row0d" : $class="row1d");
					  $i++;
					  ?>
					  <tr>
					    <td class="<?php echo $class?>"><input type="checkbox" name="students[]" class="<?php echo $class?>" value="<?php echo $number?>" /></td>
					    <td class="<?php echo $class?>"><?php echo (($last == $row["last_name"] && $first == $row["first_name"]) ? "$first $middle $last" : "$first $last") ?></td>
					  </tr>
				  	  <?
					  $match == (($last == $row["last_name"] && $first = $row["first_name"]) ? 1 : 0);
					  $last = $row["last_name"];
					  $first = $row["first_name"];
					  $middle = $row["middle_name"];
					  $number = $row["student_number"];
				  }
				  
					  (($i%2==0) ? $class="row0d" : $class="row1d");
				  	  ?>
					  <tr>
					    <td class="<?php echo $class ?>"><input type="checkbox" name="students[]" class="<?php echo $class ?>" value="<?php echo $number ?>" /></td>
					    <td class="<?php echo $class ?>"><?php echo (($match) ? "$first $middle $last" : "$first $last")?></td>
					  </tr>
						  </table>
					  <br />
					  <input type="submit" value="Submit Shortlist" />
					  </form>
	
			<?
			}
		}
	}
}
?>
