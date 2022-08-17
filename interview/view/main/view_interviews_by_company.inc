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
 | Filename: view_interviews_by_company.inc                                     |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_interviews_by_company.inc

// $company_name or $employer_id & $department_str must be available in the script that includes this file.

if ($employer_id == "") 
{
   $result = $GLOBALS['dbh']->Execute("SELECT DISTINCT e.employer_id, e.company_name 
   			  FROM employer_company as e, job_info as j
			  WHERE j.employer_id = e.employer_id AND j.department_id IN (" . $department_str . ") AND e.company_name='" . addslashes(trim($company_name)) . "'");
}
else
{
   $result = $GLOBALS['dbh']->Execute("SELECT DISTINCT e.employer_id, e.company_name 
   			  FROM employer_company as e, job_info as j
			  WHERE j.employer_id = e.employer_id AND j.department_id IN (" . $department_str . ") AND e.employer_id='" . addslashes(trim($employer_id)) . "'");
}

if ($row = $result->FetchRow())
{
   $employer_id = $row["employer_id"];
   $company_name = $row["company_name"];
}
else
{
   $employer_id = -1;
}

   $result = $GLOBALS['dbh']->Execute("
      SELECT DISTINCT j.job_id, j.job_code, d.company_name, a.int_date
      FROM interview_time AS a, job_info AS j, employer_company AS d
      WHERE d.employer_id='$employer_id' AND a.job_id=j.job_id AND j.employer_id=d.employer_id AND j.department_id IN (" . $department_str . ") 
      " . (($past) ? "" : " AND a.int_date>=CURRENT_DATE ") . "
      ORDER BY a.int_date, j.job_code, j.job_id");

   if ($result->RecordCount() == 0)
   {
      // if there are no interviews for the chosen company or the company doesn't exist
      if ($company_name != "") 
      {
         notify("There are no interviews for ".htmlentities($company_name));
         if (!$past) { echo("<a class='blue' href='$PHP_SELF&amp;select=view&amp;level1=company&amp;level2=company_name_or_employer_id&amp;employer_id=$employer_id&amp;past=true&amp;department_id=" . urlencode($department_id) . "'>Expand search to past interviews.</a>"); }
      }
      else
      {
         notify("The company could not be found.");
      }
   }   
   else      
   {
      // there are interviews on the chosen date
      echo("<h4 align='left'>Interviews for ". htmlentities($company_name).":</h4>");
                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Company</b>&nbsp;</td>
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Job Code(s)</b>&nbsp;</td>
                           <td class="rowgrey" align='center'>&nbsp;<b class="white">Date</b>&nbsp;</td>
                        </tr>
               <?php
      $rowclass = 0;
      while ($row = $result->FetchRow())
      {
         $job_id = $row["job_id"];
         $job_code = $row["job_code"];
         $company_name = $row["company_name"];
         $int_date = $row["int_date"];
         $href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . urlencode($job_id) . "&amp;department_id=" . urlencode($department_id);

         echo("<tr>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".htmlentities($company_name)."</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$int_date</a>&nbsp;</td>");
         echo("</tr>\n");
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php

      if (!$past) { echo("<a class='blue' href='$PHP_SELF&amp;select=view&amp;level1=company&amp;level2=company_name_or_employer_id&amp;employer_id=$employer_id&amp;past=true&amp;department_id=" . urlencode($department_id) . "'>Expand search to past interviews.</a>"); }
   } 

?>
