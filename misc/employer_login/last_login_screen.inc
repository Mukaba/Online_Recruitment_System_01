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
 | Filename: last_login_screen.inc                                              |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

?>
</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>"></td></tr></table>
<form action='<?php echo $PHP_SELF?>' method='post'>
<input type='hidden' name='select' value='adminemp_login' />
<input type='hidden' name='admin_employer_login' value='yes' />
<br />
<br />
<br />
<br />
<center><h3>Employer Login</h3>
<table border='0' cellpadding='5'>
  <?php if ($error){
	echo "<tr><td colspan='2'>";
 	error($error);
	echo "</td></tr>";
  }?>


<?php 
	/*  Taking out Login ID option ! 
	--------------

  <tr>
    <td align='right'>Login ID:</td>
    <td><input type='text' size='15' name='student_login_id' /></td>
  </tr>
  <tr>
    <td colspan='2' align='center'><b class='black'>OR</b></td>
  </tr>

	---------------
	*/
?>

  <tr>
    <?php
    $href = $PHP_SELF . "&amp;select=student_login&amp;order=";
    ?>

<?php 
	echo "<td colspan='2' align='center'>";

        global $auth;
        $dept_id = $auth->department;
        $sqldept = ("SELECT DISTINCT department_id FROM department
                WHERE using_full_system AND department_id = '". $auth->department."'");
        $resultdept = $GLOBALS['dbh']->Execute($sqldept);
        $rowdept=$resultdept->FetchRow();
        if ($auth->department == $rowdept['department_id']){
	$sql = ("
		SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS order_letter 
		FROM student as s, student_department as sd, student_flags_join as sfj
		WHERE
		sfj.student_flags_id='".CURRENT_FLAG."' and
		sd.record_id=sfj.record_id and
                s.student_number=sd.student_number and
		sd.department_id IN ('" . implode("', '", $departments_in_group) . "') 
		ORDER BY order_letter
		");
	} else {
	$sql = ("
		SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS order_letter 
		FROM student as s, student_department as sd
		WHERE
		s.current='1' and
                s.student_number=sd.student_number and
		sd.department_id IN ('" . implode("', '", $departments_in_group) . "') 
		ORDER BY order_letter
		");
	}
	$result = $GLOBALS['dbh']->Execute($sql);
	$rows = $result->RecordCount();
	$count = 1;
	while ($row = $result->FetchRow()) 
	{
		$count++;
		$order_choices[] = $row["order_letter"];
		echo "<a class='blue' href='$href". $row["order_letter"]."'>".$row["order_letter"]."</a>". (($count > $rows)? "" : " | ");
	}
	if (!$order) 
	{
		$order= $order_choices[0];
	}
	
	echo "</td>";
	
?>
  </tr>
  <tr>
    <td colspan='2' align='center'>
    <?php
	if ($order == ""){ $order='A'; }
        if ($auth->department == $rowdept['department_id']){
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as name 
		FROM student as s, student_department as sd, student_flags_join as sfj
		WHERE s.last_name LIKE '" . stripslashes($order) . "%' 
		AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "') 
		and sd.record_id=sfj.record_id
		and sfj.student_flags_id='".CURRENT_FLAG.".'
		AND sd.student_number=s.student_number
		ORDER BY s.last_name, s.first_name, s.student_number
		");
	} else {
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as name 
		FROM student as s, student_department as sd
		WHERE s.last_name LIKE '" . stripslashes($order) . "%' 
		AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "') 
		and s.current='1'
		AND sd.student_number=s.student_number
		ORDER BY s.last_name, s.first_name, s.student_number
		");
	}


	if ($result->RecordCount() <= 0){
		echo "No students.";
	}else{
	  $size = 10;
	  if ($result->RecordCount() < $size){
		$size = $result->RecordCount();
	  }
	  ?>
	  <select name='student_number_login2' size='<?php echo $size?>'>
	  <?php
	  $lastStudentNumber=0;
	  while($row = $result->FetchRow()){
		if ($lastStudentNumber<>$row['student_number']){
		?>
		
			<option value='<?php echo $row["student_number"]?>'>[<?php echo $row["student_number"]?>] <?php echo $row["name"]?></option>
	 	<?php
			$lastStudentNumber=$row['student_number'];
		}
	  }
	  ?>
	  </select>
	  <?php
	}
    ?>
    </td>
  </tr>
  <tr>
    <td colspan='2' align='center'><input type='submit' value='Log In' name='submit' /></td>
  </tr>
</table>
</center>
</form>
