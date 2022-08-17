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
 | Filename: names.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description: Allows the user to select a student by student number for       |
 | instances when multiple students                   have the same name        |
 | parameters: first_name - the students first name              last_name -    |
 | the students last name              return_url - the url to return to after  |
 | the student has been select                   (i.e. the form is submitted    |
 | to this url)              form_fields - optional two dimensional array that  |
 | specifies name/value pairs to be submitted                   along with the  |
 | form.                   (eg. Array(Array(name1,value1),Array(name2,value2),  |
 | ...) )  return:     student_num - the student number of the selected         |
 | student is passed back through the form as                   the value of    |
 | the form element with name student_num  notes:      If you pass a            |
 | name/value pair through the form and the value contains quotes, slashes,     |
 | etc. they                   will be returned with an escaping slash in       |
 | front of them.  You will have to remove this.              Remember to pass  |
 | first_name, last_name through if you need them              Make sure name   |
 | for name/value pairs is a valid name for an HTML form                        |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/

function select_name($first_name, $last_name, $return_url, $form_fields = Array()){
   echo("There are multiple students with the name: $first_name $last_name.  Please select the correct one by student number.");
   echo("<form action=\"$return_url\" method=\"POST\">\n");

   $hidden_fields = count($form_fields);
   for($i = 0; $i < $hidden_fields; $i++)
   {
      if ($form_fields[$i][0] != "")
      {
         echo("<input type=\"hidden\" name=\"" . $form_fields[$i][0]. "\" value=\"" . htmlspecialchars($form_fields[$i][1]) . "\">\n");
      }
   }

   echo("<table>\n");

   $result = $GLOBALS['dbh']->Execute("SELECT student_number,department_id FROM student WHERE first_name='" . addslashes($first_name) . "' AND last_name='" . addslashes($last_name) . "' ORDER BY student_number DESC");
   while($row = $result->FetchRow())
   {
      $departmentresults=$GLOBALS['dbh']->Execute("Select department_name from department where department_id=".$row['department_id']);
      $departmentrow=$departmentresults->FetchRow();
      echo("<tr>");
      echo("<td>&nbsp;" . $first_name . " " . $last_name . "&nbsp;(".$departmentrow['department_id'].")</td>");
      echo("<td><input type=\"submit\" name=\"student_num\" value=\"" . $row["student_number"] . "\"></td>");
      echo("</tr>\n");
   }

   echo("</table>\n");
   echo("</form>\n");
}

function select_name_array($student_numbers, $return_url, $form_fields = Array())
{
	global $sendEmail;
	global $conflict_studentname;
	global $conflict_studentnum;
	global $conflict_studentemail;
	global $noemail_studentname;
	global $noemail_studentnum;
	global $coming_through_again;
	global $interview;
	global $select;
	global $job_id;
	global $job_code;
	global $post;
   if($coming_through_again && ($select != 'add'))
   {
	echo("
	<table>
	<tr>
	<td>
	<h3>Edit Interview: " . $job_code . " </h3>
	</td>
	");
	echo("
	<td>
	<form method='POST' action='$PHP_SELF&PDF=1&job_id=$job_id'>
	<select name='selection'>
	    <option value='sign_up'>Sign up Students for this Interview 
	    <option value='single'>Preview Schedule
	    <option value='final'>Final Interview Schedule
	    <option value='email_schedule'>E-mail Schedule to Employer
	</select>
	&nbsp;
	<input type='image' src='misc/images/arrowg.gif' alt='go'>
	</form>
	</td>
	</tr>
	</table>
	");
	echo("
	<br />
	");
				

   }
   error("There is more than one student in the database with this name.  Please select the correct one by student number.");
   echo("<form action=\"$return_url\" method=\"POST\">\n");
   echo("<input type=\"hidden\" name=\"submit\" value=\"Time Calculator\">\n"); 
   $hidden_fields = count($form_fields);
   for($i = 0; $i < $hidden_fields; $i++)
   {
      if ($form_fields[$i][0] != "")
      {
         echo("<input type=\"hidden\" name=\"" . $form_fields[$i][0]. "\" value=\"" . htmlspecialchars($form_fields[$i][1]) . "\">\n");
      }
   }

   echo("<table>\n");

	reset($student_numbers);
	array_walk($student_numbers, 'quotestring');
	$query = "SELECT s.first_name, s.last_name, s.student_number,sd.department_id FROM student as s, student_department as sd  WHERE s.student_number=sd.student_number AND s.student_number IN (" . implode(",", $student_numbers) . ") ORDER BY s.student_number DESC";

   $result = $GLOBALS['dbh']->Execute($query);
   while($row = $result->FetchRow())
   {
      $departmentresults=$GLOBALS['dbh']->Execute("Select department_name from department where department_id=".$row['department_id']);
      $departmentrow=$departmentresults->FetchRow();
      $signedupresults=$GLOBALS['dbh']->Execute("Select distinct student_number from students_shortlisted where student_number=".$row['student_number']." and job_id=".$job_id);
      $count=0;
      $btag=FALSE;
      while ($signeduprow=$signedupresults->FetchRow()){
		$signedupstudents=$signedupstudents.$row['student_number'].",";
		$btag=TRUE;
      }
      echo("<tr>");
      if ($btag==FALSE){
      	echo("<td>&nbsp;" . $row["first_name"] . ", " . $row["last_name"] . "&nbsp;(".$departmentrow['department_name'].")</td>");
      }else{
	echo("<td><b>&nbsp;" . $row["first_name"] . ", ". $row["last_name"] . "&nbsp;(".$departmentrow['department_name'].")</b></td>");
      }
      echo("<td><input type=\"submit\" name=\"student_num\" value=\"" . $row["student_number"] . "\"></td>");
      echo("</tr>\n");
   }
   if (substr($signedupstudents,-1)==","){
                $signedupstudents=substr($signedupstudents,0,strlen($signedupstudents)-1);
		echo ("<tr><td>&nbsp;</td></tr>");
		 echo ("<tr><td>&nbsp;</td></tr>");
                echo ("<tr>");
                echo ("<td><b>".$signedupstudents." was previously shortlisted for this interview.</b></td>");
                echo ("</tr>");
   }

   echo("</table>\n");
   echo("<input type='hidden' name='duplicate' value='true'>");
   echo("<input type='hidden' name='post' value='" . $post . "'>");
   echo("<input type='hidden' name='coming_through_again' value='true'>");
   echo("<input type='hidden' name='interview' value='" . packObject(($interview)) . "'>");
   echo("<input type='hidden' name='dup_students_solved' value='true'>");
	for ($i = 0; $i < sizeof($conflict_studentname); $i++)
	{
		echo("<input type='hidden' name='conflict_studentname[".$i."]' value='".$conflict_studentname[$i]."'>");
	}
	for ($i = 0; $i < sizeof($conflict_studentnum); $i++)
	{
		echo("<input type='hidden' name='conflict_studentnum[".$i."]' value='".$conflict_studentnum[$i]."'>");
	}
	for ($i = 0; $i < sizeof($conflict_studentemail); $i++)
	{
		echo("<input type='hidden' name='conflict_studentemail[".$i."]' value='".$conflict_studentemail[$i]."'>");
	}
	for ($i = 0; $i < sizeof($noemail_studentname); $i++)
	{
		echo("<input type='hidden' name='noemail_studentname[".$i."]' value='".$noemail_studentname[$i]."'>");
	}
	for ($i = 0; $i < sizeof($noemail_studentnum); $i++)
	{
		echo("<input type='hidden' name='noemail_studentnum[".$i."]' value='".$noemail_studentnum[$i]."'>");
	}
   echo("<input type='hidden' name='sendEmail' value='".$sendEmail."'>");
   echo("<input type='hidden' name='name_array_run' value='true'>");
   echo("</form>\n");

}

function quotestring(&$element)
{
	$element = "'" . addslashes($element) . "'";
}

?>
