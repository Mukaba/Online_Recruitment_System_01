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
 | Filename: view_interviews_by_term.inc                                        |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_interviews_by_term.inc

//term_id and year must be specifed in the file that includes this one
if ($order != "job_code" && $order != "company_name" && $order != "int_date" && $order != "int_date2"){
	$order = "job_code";
}
$type = "ASC";
$int_date_url = "int_date";
if ($order == "int_date2"){
	$order = "int_date";
	$type = "DESC";
}elseif ($order == "int_date"){
	$int_date_url = "int_date2";
}
$result = $GLOBALS['dbh']->Execute("SELECT term_code, term_name FROM term WHERE term_id = '" . $term_id . "'");
$row = $result->FetchRow();
$term_code = $row["term_code"];
$term_name = $row["term_name"];

$sql = "SELECT DISTINCT j.job_id as job_id, j.job_code AS job_code, d.company_name as company_name, a.int_date as int_date
	FROM job_info AS j, interview_time AS a, employer_company AS d, interview AS z
	WHERE j.employer_id = d.employer_id 
	AND a.job_id = j.job_id 
	AND z.job_id = j.job_id
	AND NOT(z.cancelled)
	AND j.department_id IN (" . $department_str . ") 
	AND j.interview_term_id = '" . addslashes($term_id) . "' 
	AND j.interview_year = '" . addslashes($year) . "'
	ORDER BY '" . addslashes($order) . "' $type";

$result = $GLOBALS['dbh']->Execute( $sql );
$sql2 = ("
	SELECT COUNT(i.job_id) AS count 
	FROM job_info AS j, interview AS i
	WHERE j.department_id IN (" . $department_str . ") 
	AND j.interview_term_id = '" . addslashes($term_id) . "' 
	AND j.interview_year = '" . addslashes($year) . "'
	AND i.job_id=j.job_id");
$result2 = $GLOBALS['dbh']->Execute( $sql2 );
$row2 = $result2->FetchRow();
$count = $row2["count"];
 
if ($result->RecordCount() == 0)
{
	notify("There are no interviews for the selected term.");
}   
else      
{
      // there are interviews for chosen term 
      echo("<h4 align='left'>Interviews for $term_name $year: ($count)</h4>");
                $href = $PHP_SELF . "&amp;select=view&amp;level1=term&amp;level2=view&amp;term_id=" . urlencode($term_id) . "&amp;year=" . urlencode($year) . "&amp;order=";
		?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'><a href="<?php echo $href . "job_code"?>">&nbsp;<b class="white">Job Code(s)</b>&nbsp;</a></td>
                           <td class="rowgrey" align='center'><a href="<?php echo $href . "company_name"?>">&nbsp;<b class="white">Company</b>&nbsp;</a></td>
                           <td class="rowgrey" align='center'><a href="<?php echo $href . $int_date_url?>">&nbsp;<b class="white">Date</b>&nbsp;</a></td>
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
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".htmlentities($company_name)."</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$int_date</a>&nbsp;</td>");
         echo("</tr>\n");
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php

   } 

?>
