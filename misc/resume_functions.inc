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
 | Filename: resume_functions.inc                                               |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

define( SPECIFIC, 1);
define( TEMPLATE, 2);
define( GENERIC, 3);
define( DELETE, 2);
define( ACTIVE, 1);

function isValidResume($student_number, $resume_id){
	$result = $GLOBALS['dbh']->Execute("
			SELECT name
			FROM resume
			WHERE student_number = '" . addslashes($student_number) . "' AND resume_id = '" . addslashes($resume_id) . "'"
			);
	if (!result || $result->RecordCount() == 0){
		return false;
	}else{
		return true;
	}
}


function select_student($order, $order_href){ 
	global $departments_in_group;
	global $department_id;
	global $department_in_str;
	global $page;
	global $auth;
	
	if ($order == ""){
		$order="A";
	}

	if (sizeof($departments_in_group) > 1){
	?>
		<table border="0" width="100%" class="row1" cellspacing="0" cellpadding="5">
		  <tr>
		    <td align="right">Department:</td>
		    <td><select name="department_id">
			<option value="0" <?php echo ($department_id==0) ? "selected='selected'" : ""; ?>>All in Group</option>
			<?php
			foreach($departments_in_group as $dep){
				echo "\t<option value='$dep'" . (($dep == $department_id) ? "selected='selected'" : "" ) . ">" . getDepartmentName($dep) . "</option>\n";
			}
			?>
			</select>
		    	&nbsp;&nbsp;<input type="submit" Name="changetodepartment" value="Change"></td>
		</tr>
		</table>
		<br />
		<?php
	}
	
	if ($page == "add" || $page == "Add"){
		?>
		<table width="100%" class="row1" cellspacing="0" cellpadding="5" border="0">
		  <tr>
		      <td class="tableheaders">Resume Name:&nbsp;<input type="TEXT" value="" name="name"></td>
	          </tr>
		</table>
		<br />
		<?php
	}
	
	
	#$result = $GLOBALS['dbh']->Execute($sql);
?>
	<table width="100%" class="row1" cellspacing="0" cellpadding="5" border="0">
	   <tr>
	    <td colspan="2" align="LEFT" class="tableheaders">Enter a Student Number</td>
	   </tr>
	   <tr>
	     <td width="30%" align="RIGHT">Student Number:</td>
	     <td align="LEFT"><input type="TEXT" size="10" name="student_number_select"></td>
	   </tr>
	   <tr>
	     <td colspan="2" align="LEFT" class="tableheaders">-Or- Enter a Student Name</td>
	   </tr>
	   <tr>
	     <td align="RIGHT">Student Name:</td>
	     <td align="LEFT"><input type="TEXT" size="30" name="student_name_select"></td>
	   </tr>
	   <tr>
	     <td colspan="2" align="LEFT" class="tableheaders">-Or- Select a Student</td>
	   </tr>
	   <tr>
	     <td colspan="2" align="center">
<?php
	$letter_sql="	SELECT distinct SUBSTRING(s.last_name, 1, 1) AS order_letter
                	FROM student as s ,student_department as smd,student_flags_join as sfj 
			WHERE s.student_number=smd.student_number
			AND sfj.record_id=smd.record_id
			AND sfj.student_flags_id=".CURRENT_FLAG."
                	AND smd.department_id IN (".$department_in_str.")
                	ORDER BY s.last_name";	 
	$letter_results=$GLOBALS['dbh']->Execute($letter_sql);
	$letter_row=$letter_results->FetchRow();
	while ($letter_row=$letter_results->FetchRow()){
		if ($firstletter==NULL){
			$firstletter=strtoupper($letter_row['order_letter']);			
		}
	}
	if ($order=='A'){
		if ($firstletter<>NULL){
			$order=$firstletter;
		}else{
			
			if ($firstletter==NULL){
				notify("There are no students in your department");
			}
		}
	}

        $sql = "SELECT s.student_number, smd.department_id, CONCAT(s.first_name,' ',s.last_name) as name
                FROM student as s LEFT JOIN student_department as smd ON s.student_number=smd.student_number, student_flags_join as sfj
                WHERE s.last_name LIKE '" . $order . "%' AND sfj.record_id=smd.record_id AND sfj.student_flags_id=".CURRENT_FLAG." AND (smd.department_id IN (".$department_in_str."))
                ORDER BY s.last_name";

        $result = $GLOBALS['dbh']->Execute($sql);

	$letter_results=$GLOBALS['dbh']->Execute($letter_sql);
	while ($letter_row=$letter_results->FetchRow()){
		if (strtolower($lastLetter)<>strtolower($letter_row['order_letter'])){
?>
	       		<a CLASS = "blue" href="<?php echo ($order_href) ?>&amp;order=<?php echo $letter_row['order_letter']?>"><?php echo strtoupper($letter_row['order_letter'])?></a> | 
<?php
		}
		
	 	$lastLetter=$letter_row['order_letter'];
	}
?>
	     </td>
	   </tr>
	   <tr>
	     <td colspan="2"  align="center">
	       <?php
		    $size=10;
		    if ($result->RecordCount() < 10){
			    $size = $result->RecordCount();
		    }
		    if ($firstletter<>NULL){
		    ?>
		    	<select name="student_num" size="<?php echo $size?>">
		    <?php
		    		while ($row = $result->FetchRow()){
					if (($row["department_id"] == $auth->department) && (sizeof($departments_in_group)>1)) {
		   	   ?>
			   		<option value="<?php echo $row["student_number"]?>">*[<?php echo $row["student_number"]?>] <?php echo $row["name"]?></option>
				<?php
					} else {
				?>
			   		<option value="<?php echo $row["student_number"]?>">[<?php echo $row["student_number"]?>] <?php echo $row["name"]?></option>
			   <?php
			   		}
		    		}
		    ?>
	            	</select>
<?php
		    }
?>
	    </td>
	  </tr>
	  <?php
		if (sizeof($departments_in_group)>1) {
	?>
	  <tr><td colspan=2 align=center>
	Note: students with an asterisk (*) are in your department.
	    </td>
	  </tr>
	  <?php
	  	}
	?>
	</table>
<?php
}

function find_student($student_name_select, $student_number_select, $student_num){

	global $department_in_str;
	
	if ($student_number_select != ""){
		$student_num = $student_number_select;
	}

	//entered a name, but not a student number in the text boxes
	if ($student_name_select != "" && $student_number_select == ""){
		$sql = "SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as name
			FROM student as s, student_department as sd
			WHERE CONCAT(s.first_name, ' ', s.last_name) = '" . addslashes(stripslashes($student_name_select)) . "'
				AND sd.department_id IN (" . $department_in_str . ")
				AND s.student_number = sd.student_number";
		$result = $GLOBALS['dbh']->Execute($sql);
		$num = $result->RecordCount();
		if( $num <= 0 ){
			return array( -1, "Student name - " . $student_name_select . " - does not exist in selected department." );
		}elseif( $num == 1 ){
			$row =  $result->FetchRow();
			return array( $row["student_number"], "" );
		}else{
			return array( -1, "duplicate" );
		}
	}else{
		if( $student_num == "" ){
			return array( -1, "Please select a student by either entering their name or student number, or by selecting them from the list in the combo box below." );
		}elseif( !isValidStudentNumber( $student_num, $department_in_str ) ){
			return array( -1, "Student number - " . $student_num . " - does not exist in selected department." );
		}else{
			return array( $student_num, "" );
		}
	}
}
?>
