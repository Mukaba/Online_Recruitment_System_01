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
 | Filename: delete_main.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Shows the user a list of all resumes in the system.  If no      |
 | jobs are associated with a given resume allow the user to click on the       |
 | resume to delete it otherwise they are not allowed to delete the resume.     |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Resumes - Delete</h3>
<?php

$sql = "SELECT r.name, DATE_FORMAT(r.last_modified, '%b %d, %Y %H:%i') as last_modified, r.resume_id, c.description
    FROM resume as r LEFT JOIN coverletter_type as c on r.type_id = c.type_id
    WHERE r.student_number = '" . addslashes($student_number) . "'
    ORDER BY r.type_id DESC, r.name";

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0){
    echo "<p>No resumes current in the system.  To add a resume click the 'Add' option under the 'Resume' menu.</p>";
}else{
    ?>
    <form method="post" action="<?php echo $PHP_SELF?>">
    <input type="hidden" name="select" value="resume" />
    <table cellpadding="5" class='backgrey'  border="0">
      <tr>
        <td><p><b>Note:</b> Only resumes that have no jobs associated<br />with them can be deleted.</p></td>
      </tr>
    </table><br />

<table cellspacing='0' cellpadding='0' border='1'>
<tr>
  <td>

     <table border='0' cellpadding='2'>
      <tr>
        <td class="rowgrey"><b class="white">&nbsp;Resume</b></td>
        <td class="rowgrey"><b class="white">&nbsp;Type</b></td>
        <td class="rowgrey"><b class="white">&nbsp;Last Modified</b></td>
        <td class="rowgrey"><b class="white">&nbsp;Jobs</b></td>
      </tr>

      <?php
      $i = 0;
      while ($row = $result->FetchRow()){
          (($i % 2 == 0) ? $class="row0d" : $class="row1d");
          $i++;
          
          $sql = "SELECT job_code
                            FROM job_info as ji, applications as a
                            WHERE a.job_id=ji.job_id AND a.student_number='$student_number' AND a.resume_id='".$row["resume_id"]."'
                ORDER BY ji.job_code";

          $result_jobs = $GLOBALS['dbh']->Execute( $sql );
          $jobs = "";
          $job_count =0;
          
          while ($row_jobs = $result_jobs->FetchRow()){
            $jobs .= $row_jobs["job_code"] . "<br />";
            $job_count++;
          }

          //remove last <br /> tag
          $jobs = substr($jobs, 0, strlen($jobs) - 6);
          
          $href = $PHP_SELF . "&amp;select=resume&amp;page=Delete&amp;resume_id=" . $row["resume_id"];
          
          ?>
          <tr>
            <?php 
            if ($job_count > 0)
            {
            ?>
            <td valign='top' class="<?php echo $class?>">&nbsp; <?php echo $row["name"]?>&nbsp;</td>
            <?php 
            }else{
            ?>
            <td valign='top' class="<?php echo $class?>">&nbsp; <?php echo (($row["name"] == "" ) ? "&nbsp" : "<a class='blue' href='$href'>" . $row["name"] . "</a>" )?>&nbsp;</td>
            <?php
            }
            ?>
            <td valign='top' class="<?php echo $class?>">&nbsp; <?php echo $row["description"]?>&nbsp;</td>
            <td valign='top' class="<?php echo $class?>">&nbsp; <?php echo $row["last_modified"]?>&nbsp;</td>
            <td valign='top' class="<?php echo $class?>"><?php echo (($jobs == "" ) ? "(none)" : $jobs)?>&nbsp;</td>
          </tr>
      <?php
      }
      ?>
      </table>
    </td>
    </tr>
</table>
</form>
      <?php
}
?>
