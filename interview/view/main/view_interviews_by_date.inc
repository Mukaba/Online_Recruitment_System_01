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
 | Filename: view_interviews_by_date.inc                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_interviews_by_date.inc
// $date, $department must be sent by the script that includes this file.


   $int_date = $date;
   $result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT j.job_id,j.job_code, d.company_name, z.emailed_employer, z.student_visible
	FROM interview_time AS a, job_info AS j, employer_company AS d, interview AS z
	WHERE a.int_date='$int_date' 
	AND a.job_id=j.job_id
	AND j.department_id IN (" . $department_in_str . ")
	AND j.employer_id=d.employer_id
	AND z.job_id=j.job_id
	AND NOT(z.cancelled)
	ORDER BY d.company_name, j.job_code, j.job_id");

   if ($result->RecordCount() == 0)
   {
      // if there are no interviews on chosen date, indicate this and go back to the select date screen
      notify("There are no interviews on ".$int_date);
   }
   else      
   {
      // there are interviews on the chosen date
      echo("<h4 align='left'>Interviews on ". $int_date.":</h4>");
                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Company</b>&nbsp;</td>
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Job Code(s)</b>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<b class="white">Posted</b>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<b class="white">Final Schedule</b>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<b class="white">E-mail Employer</b>&nbsp;</td>
                        </tr>
               <?php
      $rowclass = 0;
      while ($row = $result->FetchRow())
      {
         $job_id = $row["job_id"];
         $job_code = $row["job_code"];
         $company_name = $row["company_name"];
	 $emailed_employer = $row["emailed_employer"];
	 $student_visible = $row["student_visible"];

	 $lunch_sql="Select c.first_name,c.last_name from contact as c, interview_time as it WHERE c.contact_id=it.student_number AND it.job_id=".$job_id;
	 $lunch_results=$GLOBALS['dbh']->Execute($lunch_sql);
	 $lunch_row=$lunch_results->FetchRow();
	 if ($lunch_results->RecordCount() > 0){
		$lunch=$lunch_row['first_name']." ".$lunch_row['last_name'];
	 }else{
		$lunch="&nbsp;";
	 }

	$href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . urlencode($job_id) . "&amp;department_id=" . urlencode($department_id);

         echo("<tr><td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");
	 echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
	 echo(($student_visible) ? "<img src='misc/images/smchk_bl.gif' alt='' /></td>\n" : "&nbsp;</td>\n");
	 echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
	 echo((isSignUpComplete($job_id) ? "&nbsp;<a href='" . $PHP_SELF . "&amp;PDF=1&amp;selection=final&amp;job_id=".$job_id."'><img src='misc/images/calendar2.gif' border='0' alt='' /></a>&nbsp;</td>\n" : "&nbsp;</td>\n"));
	 echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
	 echo((($emailed_employer) ? "<img src='misc/images/email.gif' alt='' /></td></tr>\n" : "&nbsp;</td></tr>\n"));
	 
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php

   } 

?>
