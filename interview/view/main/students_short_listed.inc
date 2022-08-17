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
 | Filename: students_short_listed.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: display the list of students shortlisted for a job              |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/

?>

<table cellspacing="0" cellpadding="0" border="1"><tr><td>
   <table border="0" cellpadding="2">
      <tr>
         <td class="rowgrey" align="center">&nbsp;<b class="white">Students shortlisted</b>&nbsp;</td><td class="rowgrey" align="center"><b class='white'>Department</b></td>
      </tr>   

<?php

$result2 = $GLOBALS['dbh']->Execute("
   SELECT CONCAT(s.first_name, ' ', s.last_name) AS student_name, s.student_number,d.department_name as department_name
   FROM student AS s
   INNER JOIN student_department AS sd
   ON s.student_number = sd.student_number 
   INNER JOIN students_shortlisted AS ss
   ON sd.student_number=ss.student_number
   INNER JOIN department AS d
   ON d.department_id=sd.department_id
   WHERE ss.job_id='" . addslashes($job_id) . "' 
   ORDER BY s.last_name, s.first_name
   ");

$rowcount = 5;

$intRow=-1;
$lastStudentNumber=0;
while ($row=$result2->FetchRow()){
    if ($lastStudentNumber<>$row['student_number']){
        $intRow++;
        $arrStudentsShortlisted[$intRow][0]=$row['student_name'];
        $arrStudentsShortlisted[$intRow][1]=$row['department_name'];
        $lastStudentNumber=$row['student_number'];
    } else {
        $arrStudentsShortlisted[$intRow][1]=$arrStudentsShortlisted[$intRow][1].", ".$row['department_name'];
    }
}
$rowcount=0;
while ($rowcount<=$intRow)
{
    echo("<tr><td class='" . (($rowcount % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $arrStudentsShortlisted[$rowcount][0] . "&nbsp;</td><td class='" . ((($rowcount) % 2) ? "row0d" : "row1d") . "'>&nbsp;". $arrStudentsShortlisted[$rowcount][1]. "</td></tr>");
    $rowcount++;
}

?>

   </table>
</td></tr></table>
