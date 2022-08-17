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
 | Filename: view_interviews_by_student.inc                                     |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
// filename: view_interviews_by_student.inc

// $student_name or $student_num  &department_str must be available in the script that includes this file.
if ($student_num == "") 
{
    /*
       Commented out by: Jon
       Date: October 28, 2004
       Reason: This code seemed like it was trying to handle multiple students but it wasn't working
       so I added a cleaner looking single student handler below

       $names[0] = trim($student_name);
       $names = break_names($names);
       $len_names_d1 = sizeof($names);
       for($i = 0; $i < $len_names_d1; $i++)
       {
       $len_names_d2 = sizeof($names[$i]);
       for($j = 0; $j < $len_names_d2; $j++)
       {
       $names_array[] = "(s.first_name='" . addslashes($names[$i][$j]["first_name"]) . "' AND s.last_name='" . addslashes($names[$i][$j]["last_name"]) . "')";
        }
        }
        $names = implode(" OR ", $names_array);
        $query = "SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) AS student_name 
        FROM student as s, student_department as sd 
        WHERE " . $names . " AND sd.department_id IN (" . $department_str . ") and sd.student_number=s.student_number ORDER BY s.student_number";

        $result = $GLOBALS['dbh']->Execute($query);
     */

    $sql = ("
            SELECT DISTINCT s.student_number, sd.record_id, CONCAT(s.first_name,' ',s.last_name) AS student_name
            FROM student AS s
            INNER JOIN student_department AS sd 
            ON sd.student_number=s.student_number
            WHERE sd.department_id IN (" . $department_str . ")
            ");

    if ($first_name)
    {
        $sql .= ("                                                                                                                       
                AND s.first_name LIKE '" . addslashes($first_name) . "%'                                                                                 
                ");
    }

    if ($last_name)
    {
        $sql .= ("                                                                                                                       
                AND s.last_name LIKE '" . addslashes($last_name) . "%'                                                                                   
                ");
    }

    $result = $GLOBALS['dbh']->Execute($sql);
    

/*	
   $names = split(" ", trim($student_name));
   if (count($names) == 3)
   {
      $first_name = $names[0];
      $last_name = $names[2];
   }
   elseif (count($names) == 2)
   {
      $first_name = $names[0];
      $last_name = $names[1];
   }
   else
   {
      $first_name = "-1";
      $last_name = "-1";
   }
   $result = $GLOBALS['dbh']->Execute("SELECT s.student_number, CONCAT(first_name, ' ', last_name) AS student_name 
   			  FROM student
			  WHERE first_name='" . addslashes(trim($first_name)) . "' AND last_name='" . addslashes(trim($last_name)) . "' ORDER BY student_number");

	
   $sql=("SELECT s.student_number, CONCAT(first_name, ' ', last_name) AS student_name 
   			  FROM student
			  WHERE first_name='" . addslashes(trim($first_name)) . "' AND last_name='" . addslashes(trim($last_name)) . "' ORDER BY student_number");

*/
}
else
{
   $result = $GLOBALS['dbh']->Execute("SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) AS student_name 
   			FROM student as s, student_department as sd  
			WHERE sd.department_id IN (" . $department_str . ") AND s.student_number='" . addslashes(trim($student_num)) . "' and sd.student_number=s.student_number");

   $sql = ("SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) AS student_name 
   			FROM student as s, student_department as sd  
			WHERE sd.department_id IN (" . $department_str . ") AND s.student_number='" . addslashes(trim($student_num)) . "' and sd.student_number=s.student_number");
	
}
// if more than one student name result found - NEW: added == instead of =
if ($result->RecordCount() > 1 && $student_num == "")
{
   include("misc/names.inc");
	while ($row = $result->FetchRow())
	{
		$student_numbers[] = $row["student_number"];
	}

   select_name_array($student_numbers, $PHP_SELF, Array(Array("select", "view"), Array("level1", "student"), Array("level2", "student_num_or_student_name"), Array("department_id", $department_id)));
}
else
{
   if ($row = $result->FetchRow())
   {
      $student_num = $row["student_number"];
      $student_name = $row["student_name"];
   }
   else
   {
      $student_num = -1;
   }


   $sql = ("
      SELECT j.job_id, j.job_code, d.company_name, e.int_date, e.int_time, MAX(z.int_date)
      FROM students_shortlisted AS a, job_info AS j, employer_company AS d, interview_time AS z
      LEFT OUTER JOIN interview_time AS e
      ON a.job_id=e.job_id AND e.student_number='$student_num' AND e.switch=0
      WHERE a.student_number='$student_num' 
      AND a.job_id=j.job_id 
      AND z.job_id=a.job_id
      AND j.employer_id=d.employer_id
      " . (($past) ? "" : " AND ((e.int_date>=CURRENT_DATE) OR IF(ISNULL(e.int_date), (z.int_date>=CURRENT_DATE), 0))") . "
      GROUP BY j.job_id
      ORDER BY	e.int_date, e.int_time, d.company_name, j.job_code, j.job_id
    ");
    $result = $GLOBALS['dbh']->Execute($sql);

   if ($result->RecordCount() == 0)
   {
      // if there are no interviews for the chosen student or the student doesn't exist
      if ($student_name != "") 
      {
         notify("There are no interviews for ".$student_name);
         if (!$past) { echo("<a class='blue' href='$PHP_SELF&amp;select=view&amp;level1=student&amp;level2=student_num_or_student_name&amp;student_num=$student_num&amp;past=true&amp;department_id=" . urlencode($department_id) . "'>Expand search to past interviews.</a>"); }
      }
      else
      {
         echo("The student could not be found.");
      }
   }   
   else      
   {
      // there are interviews on the chosen date
      echo("<h4 align='left'>Interviews for " . $student_name . " (" . $student_num . "):</h4>");
                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align="center">&nbsp;<b class="white">Company</b>&nbsp;</td>
                           <td class="rowgrey" align="center">&nbsp;<b class="white">Job Code(s)</b>&nbsp;</td>
                           <td class="rowgrey" align="center">&nbsp;<b class="white">Date</b>&nbsp;</td>
                           <td class="rowgrey" align="center">&nbsp;<b class="white">Time</b>&nbsp;</td>
                        </tr>
               <?php
      $rowclass = 0;
      while ($row = $result->FetchRow())
      {
         $job_id = $row["job_id"];
         $job_code = $row["job_code"];
         $company_name = $row["company_name"];
         $int_date = $row["int_date"];

         $int_time = $row["int_time"];
         if ($int_time != "")
         {
            $splittime = explode(":", $int_time);             
            $int_time = date("g:i a", mktime($splittime[0], $splittime[1], 0));            
            if (strlen($int_time) < 8) { $int_time = "&nbsp;&nbsp;" . $int_time; }            
         }

         $href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;department_id=0&amp;job_id=" . urlencode($job_id);

         echo("<tr>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>");

         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$int_date</a>&nbsp;</td>");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">$int_time</a>&nbsp;</td>");
         echo("</tr>\n");
         $rowclass++;
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php
      if (!$past) { echo("<a class='blue' href='$PHP_SELF&amp;select=view&amp;level1=student&amp;level2=student_num_or_student_name&amp;student_num=$student_num&amp;past=true&amp;department_id=" . urlencode($department_id) . "'>Expand search to past interviews.</a>"); }
   } 
}
?>
