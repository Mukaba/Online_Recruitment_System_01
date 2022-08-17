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
 | Description: Displays a form where the user can specify which coverletters   |
 | they wish to mark as deleted from the system.                                |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Delete Cover Letters</h3>

<form action="<?php echo $PHP_SELF?>" method="post">
<input type="hidden" name="select" value="coverletter" />
<table border="0" cellpadding="3" cellspacing="0" class="row1">
  <tr>
    <td>
      <table border="0" width="100%" cellspacing="0" cellpadding="3">
	<?php
	if (sizeof($departments_in_group) > 1){
		?>
		<tr>
		  <td>Department:</td>
		  <td colspan="3"><select name="department_id">
		  		  <option value="0" <?php echo ($department_id == 0)? "selected='selected'" : "" ?>>All In Group</option>
		  <?php
		  foreach($departments_in_group as $dep){
			  echo "\t<option value='$dep'" . (($dep == $department_id) ? " selected='selected' " : "") . ">" . getDepartmentName($dep) . "</option>\n";
		  }
		  ?>
		  </select>
		  </td>
		</tr>
		<tr>
		  <td colspan="4"><hr width="100%" /></td>
		</tr>
		<?php
	}
	?>
	
	<tr>
	  <td>Job Code:</td>
    	  <td><input type="text" size="20" name="job_code" value="<?php echo $job_code?>" /></td>
	  <td align="right">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Term:&nbsp;&nbsp;</td>
	  <td><select name="term_id">
	  <?php
	  $result = $GLOBALS['dbh']->Execute("SELECT term_id, term_code FROM term");
	  while ($row = $result->FetchRow()){
		  echo "\t<option value='" .  $row["term_id"] . "'" . (($row["term_id"] == $term_id) ? "selected='selected'" : "") . ">" . $row["term_code"] . "</option>\n";
	  }
	  ?>
	    </select>
	  </td>
	</tr>
	<tr>
          <td>Student Number:&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td><input type="text" name="student_num" value="<?php echo $student_num?>" size="10" /></td>
	  <td align="right">Year:&nbsp;&nbsp;</td>
	  <td><select name="year">
	  <?php
	  $result = $GLOBALS['dbh']->Execute("SELECT MIN(year) as start, DATE_FORMAT(CURRENT_DATE, '%Y') as end FROM job_info");
	  $row = $result->FetchRow();
	  for ($i = $row["start"]; $i <= $row["end"]; $i++){
		  if (($i == $row["end"] && $year == "") || ($i == $year)){
			  ?>
			  <option value="<?php echo $i?>" selected='selected'><?php echo $i?></option>
			  <?php
		  }else{
			  ?>
			  <option value="<?php echo $i?>"><?php echo $i?></option>
			  <?php
		  }
	  }
	  ?>
	    </select>
	  </td>
	</tr>

      </table>
    </td>
  </tr>
  <tr>
    <td><hr width="100%"></td>
  </tr>
  <tr>
    <td>
      <table border="0" cellspacing="0" cellpadding="1">
        <tr>
	  <td colspan="2">Options:</td>
	</tr>
	<tr>
   	  <td><input type="radio" class="<?php echo $row1 ?>" name="delete" value="job_all" <?php echo (($delete == "job_all") ? "checked='checkedd'" : "")?>  /></td>
    	  <td>Delete all cover letters for selected JOB</td>
	</tr>
	<tr>
	  <td><input type="radio" class="row1" name="delete" value="student_all" <?php echo (($delete == "student_all") ? "checked'checkedd'" : "") ?>  /></td>
	  <td>Delete all cover letters for selected STUDENT</td>
	</tr>
	<tr>
  	  <td><input type="radio" class="row1" name="delete" value="term_all" <?php echo (($delete == "term_all") ? "checked'checkedd'" : "" ) ?> /></td>
   	  <td>Delete all cover letters for selected TERM</td>
 	</tr>
	<tr>
	  <td><input type="radio" class="row1" name="delete" value="job_student" <?php echo (($delete == "job_student") ? "checked'checkedd'" : "" ) ?> /></td>
  	  <td>Delete cover letter for selected JOB and STUDENT</td>
 	</tr>
      </table>
    </td>
  </tr>
  <tr>
    <td><hr width="100%"></td>
  </tr>
  <tr>
    <td><input type="submit" name="page" value="Delete" /></td>
  </tr>
</table>
</form>
