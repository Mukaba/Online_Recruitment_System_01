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
 | Filename: view_by_term_main.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

//filename: view_by_term_main.inc

?>

<form action="<?php echo $PHP_SELF?>" method="post">
<input type="hidden" name="select" value="view" />
<input type="hidden" name="level1" value="term" />

<table class='row1' border="0" cellpadding="4" cellspacing="0">
  <tr>
    <td>
      <table border="0" width="100%">
        <tr>
	  <td>Term:</td>
	  <td><select name="term_id">
	    <?php 
	    $sql = ("
		SELECT term_id, term_name, 
		IF (start_date <= DATE_FORMAT(CURRENT_DATE(), '%m') && end_date >= DATE_FORMAT(CURRENT_DATE(), '%m'), 1, 0) AS current
	    	FROM term
		");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow()){
			if ($row["current"]){
				?>
				<option value="<?php echo $row["term_id"]?>" selected='selected'><?php echo $row["term_name"]?></option>
				<?php
			}else{
				?>
				<option value="<?php echo $row["term_id"]?>"><?php echo $row["term_name"]?></option>
				<?php
			}
		}
	     ?>
	     </select>
	   </td>
	 </tr>
	 <tr>
	   <td>Year:</td>
	   <td><select name="year">
	     <?php
	     if($auth->department == TRAFFIC_DIRECTOR_DEPARTMENT)
         {
             $sql = "SELECT MIN(ji.interview_year) as min_year, MAX(ji.interview_year) as max_year
                 FROM job_info AS ji
                 INNER JOIN department AS d
                 ON ji.department_id=d.department_id
                 WHERE ji.interview_year != 0 
                 AND d.job_list
                 ";
	     }
         else
         {
             $sql = "SELECT MIN(interview_year) as min_year, MAX(interview_year) as max_year
                 FROM job_info
                 WHERE interview_year != 0 
                 AND department_id=$auth->department
                 ";
         }
         $result = $GLOBALS['dbh']->Execute($sql);
	     $row = $result->FetchRow();
	     $max_year = $row["max_year"];

         if ($row["min_year"] == NULL) {
	       $begin_year = $row["max_year"];
	     } else {
	       $begin_year = $row["min_year"];
	     }
	     for($i = $max_year; $i>= (int)$begin_year; $i--){
		     if ($i == $max_year){
			     ?>
			     <option value="<?php echo $i;?>" selected='selected'><?php echo $i;?></option>
			     <?php
		     }else{
			     ?>
			     <option value="<?php echo $i;?>"><?php echo $i;?></option>
			     <?php
		     }
	     }
	     ?>
	     </select>
	   </td>
	 </tr>
	<?php
    if($auth->department == TRAFFIC_DIRECTOR_DEPARTMENT)
    {
        $sql = "SELECT DISTINCT d.department_id, d.department_name 
            FROM job_info AS ji
            INNER JOIN department AS d
            ON ji.department_id=d.department_id
            WHERE ji.interview_year != 0 
            AND d.job_list
            ORDER BY department_name
            ";
            $result = $GLOBALS['dbh']->Execute($sql);
        ?>
    <tr>
        <td>Department:</td>
        <td><select name="department_id">
        <?php 
        echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All of UVic</option>");
        while ($row = $result->FetchRow())
        {       
            if ($row["department_id"] == $department_id)
            {
                echo "<option selected='selected' value='" . $row["department_id"] . "'>" . $row["department_name"] . "</option>\n";
            }
            else
            {
                echo "<option value='" . $row["department_id"] . "'>" . $row["department_name"]. "</option>\n";
            }

        }
    }
	elseif (sizeof($departments_in_group)>1){
	?>
	<tr>
	  <td>Department:</td>
	  <td><select name="department_id">
                <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach($departments_in_group as $department){
			if ($department == $department_id){
				echo "<option selected='selected' value='" . $department . "'>" . getDepartmentName($department) . "</option>\n";
			}else{
				echo "<option value='" . $department . "'>" . getDepartmentName($department) . "</option>\n";
			}
		}
		?>
	      </select>
	  </td>
	</tr>
	<?php
	}
	?>
	<tr>
	   <td colspan="2" align='center'><hr /><input type="submit" name="submit" value="View Interviews" /></td>
	 </tr>
      </table>
    </td>
  </tr>
</table>
</form>
