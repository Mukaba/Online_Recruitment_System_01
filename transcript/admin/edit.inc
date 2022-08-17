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
 | Filename: edit.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: Ensures the selected student is valid, gets there transcript    |
 | (if in the db.), and brings up a for allowing the user to edit the           |
 | transcript for the selected student.                                         |
 +------------------------------------------------------------------------------+

*/



list($student_num, $error) = find_student($student_name_select, $student_number_select, $student_num);

if ($student_num == -1){
	if ($error == "2"){
		include( "misc/names.inc" );
		list($first, $last) = split (" ", $student_name_select);
		select_name($first, $last, $PHP_SELF . "&amp;select=transcript&amp;page=" . urlencode($page) . "&amp;PDF=1");
	}else{
		error($error);
		include ("transcript/admin/view_main.inc" );
	}
}else{
	if ($student_name_select == ""){
		$student_name_select = getStudentName($student_num);
	}

	$sql = "SELECT transcript, DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified
		FROM transcript
		WHERE student_number = '" . addslashes(stripslashes($student_num)) . "'";
	$result = $GLOBALS['dbh']->Execute( $sql );

		$row = $result->FetchRow();

		?>
		<h3>Student Information - Edit Transcript</h3>
		<form method="post" action="<?php echo $PHP_SELF?>">
		<input type="hidden" name="select" value="transcript" />
		<input type="hidden" name="student_num" value="<?php echo $student_num?>" />
		<input type="hidden" name="department_id" value="<?php echo $department_id?>" />
		<input type="hidden" name="PDF" value="1" />
		<table border="0" width="100%" cellpadding="5">
		  <tr>
		    <td>
		        Last Modified: <?php echo $row["last_modified"]?>
			<hr width="100%" />
		    </td>
		  </tr>
		  <tr>
		    <td>
		      <table border="0" class="row1" cellspacing="0" cellpadding="5">
		        <tr>
		          <td align="left" >Student Number: <?php echo $student_num?><br />
		          Student Name: <?php echo $student_name_select?></td>
		        </tr>
		      </table>
		    </td>
		  </tr>
		  <tr>
		    <td align="center">
	  	      <table border="0" width="100%" cellspacing="0" class="row1">
			<tr>
		          <td><textarea name="transcript" cols="80" rows="30"><?php echo $row["transcript"];?></textarea></td>
		        </tr>
			<tr>
			  <td>
			    <table border="0" width="100%" cellspacing="0" cellpadding="10">
			      <tr>
			        <td width="50%" align="right"><input type="submit" name="page" value="Save" /></td>
				<td width="50%" align="left"><input type="reset" value="Reset" /></td>
			      </tr>
			    </table>
			  </td>
			</tr>
		      </table>
		    </td>
		  </tr>
		</table>
		</form>
		<?php
}
?>
		
