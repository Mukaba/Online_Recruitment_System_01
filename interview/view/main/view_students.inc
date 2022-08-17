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
 | Filename: view_students.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: Give the user a list of students to choose from                 |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
if ($department_id == ""){
	$department_id = $auth->department;
}elseif ($department_id != 0 && !in_array($department_id, $departments_in_group)){
	error("You do not have access to that department.");
	$department_id = $auth->department;
}

$department_in_str = "";
if($department_id == "0"){
	$department_in_str = "'" . implode("', '", $departments_in_group) . "'";
}else{
	$department_in_str = "'" . $department_id . "'";
}

$first_name = trim($first_name);
$last_name = trim($last_name);
$student_num = trim($student_num);
$year = trim($year);

/*
 First off, we need to find which letters to allow the user to click on across the
 screen.  If there's no companies in the database for the letter Q, there's no
 point in showing that as an option to click on.
*/
$sql = ("
        SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS order_letter
        FROM student as s
        WHERE s.last_name IS NOT NULL
        AND s.last_name !=''
        ORDER BY s.last_name
        ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
        $order_choices[] = $row["order_letter"];
}

if (!$order)
{
        $order = $order_choices[0];
}

?>

<form action="<?php echo($PHP_SELF); ?>" method=post>
   <input type="hidden" name="select" value="view" />
   <input type="hidden" name="level1" value="student" />
   <input type="hidden" name="level2" value="student_list" />
   <input type="hidden" name="past" value="<?php echo($past); ?>" />

   <table border="1" cellpadding="0" cellspacing="0"><tr><td><table>
      <tr>
         <td>First Name:</td>
         <td><input type="text" name="first_name" value="<?php echo($first_name); ?>" size="15" maxlength="40" /></td>
         <td>Student Number:</td>
         <td><input type="text" name="student_num" value="<?php echo($student_num); ?>" size="10" maxlength="10" /></td>
      </tr>
      <tr>
         <td>Last Name:</td>
         <td><input type="text" name="last_name" value="<?php echo($last_name); ?>" size="15" maxlength="40" /></td>
         <td>Registration Year:</td>
         <td><input type="text" name="year" value="<?php echo($year); ?>" size="4" maxlength="4" /></td>
      </tr>
      <?php
      if (sizeof($departments_in_group) <= 1){
	      ?>
	      <input type="hidden" name="department_id" value="<?php echo key($departments_in_group)?>" />
	      <?php
      }else{
      ?>
        <tr>
	  <td>Department:</td>
	  <td colspan="3"><select name="department_id">
                <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach ($departments_in_group as $department){
			if ($department == $department_id){
				?>
				<option selected='selected' value="<?php echo $department?>"><?php echo getDepartmentName($department)?></option>
				<?php
			}else{
				?>
				<option value="<?php echo $department?>"><?php echo getDepartmentName($department)?></option>
				<?php
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
         <td colspan="4" align="center"><input type="submit" name="level3" value="Search" /></td>
      </tr>
   </table></td></tr></table>
</form>

<?php
$href = $PHP_SELF . "&amp;select=view&amp;level1=student&amp;level2=student_list&amp;department_id=" . urlencode($department_id) . "&amp;past=" . urlencode($past) . "&amp;order=";
?>
<form method="post" action="<?php echo( $PHP_SELF )?>">
   <input type="hidden" name="select" value="view" />
   <input type="hidden" name="level1" value="student" />
   <input type="hidden" name="department_id" value="<?php echo $department_id?>" />
   <input type="hidden" name="level2" value="student_num_or_student_name" />
   <input type="hidden" name="past" value="<?php echo( $past )?>" />
   <table border="0">
     <tr>
       <td align="center">
	<?php
	for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
	{
		echo("<a class='blue' href='" . $href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
	}
  	echo("<a class='blue' href='" . $href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
	?>
       </td>
    </tr>
   <tr>
     <td align="center">
<?php

if ($where == "" && $order == ""){
	$order = "A";
}

if ($where == ""){
	$sql = "SELECT s.student_number, CONCAT(s.first_name, \" \", s.last_name) AS name 
		FROM student as s, student_department as sd 
		WHERE sd.department_id IN (" . $department_in_str . ") 
		AND s.last_name LIKE '" . $order . "%' 
		AND sd.student_number=s.student_number
		AND s.current='1'
		ORDER BY s.last_name, s.first_name";
}else{
	$sql = "SELECT student_number, CONCAT(first_name, \" \", last_name) AS name 
		FROM student WHERE " . $where . " 
		AND department_id IN (" . $department_in_str . ") 
		AND current='1'
		ORDER BY last_name, first_name";
}
$result = $GLOBALS['dbh']->Execute( $sql );

$num_students =  $result->RecordCount();
if ( $num_students > 0){
	if ($num_students < 10){
		$size = $num_students;

        // hack to prevent browser from automatically choosing the first student in the select box when we have one student
        // returned from the query 
        if ($size == 1) {
            $size++;
        }
	}else{
		$size = 10;
	}
	echo "<select size=\"$size\" name=\"student_num\">";
	$lastStudentNumber=0;
	while($row =  $result->FetchRow()){
		if ($lastStudentNumber<>$row['student_number']){
			echo "<option value=\"" . $row["student_number"] . "\">[" . $row["student_number"] . "] " .  $row["name"] . "</option>\n";
		}
		$lastStudentNumber=$row['student_number'];
	}
	echo "</select>";
}else{
	echo "No students in selected query";
}
?>
       </td>
     </tr>
     <tr>
       <td align="center"><input type="submit" value="View Interviews" /></td>
     </tr>
   </table>
</form>


<?php

// function whereClauseYear($year, $table_prefix)
// purpose: Generates statements for the where clause of a query to select students from a certain year.
//    $year should be the students' year.
// example. the year is 2000, whereClauseYear('00', 'a') returns
//          "CHARACTER_LENGTH(a.student_number)=7 AND SUBSTRING(a.student_number, 1, 2)='00'"
function whereClauseYear($year, $table_prefix)
{
   $clause = "";
   $year = trim($year);
   if ($year != "")
   {
      $table_prefix = trim($table_prefix);

      if (strlen($year) == 2)
      {
         $cur_year = date("Y");
         $year += $cur_year - ($cur_year % 100);
         if ($cur_year < $year) { $year -= 100;   /* use last century */ }
      }
      if ($table_prefix != "") { $table_prefix .= "."; }

      $clause = " CHARACTER_LENGTH(" . $table_prefix . "student_number)=";

      if ($year <= 1988) { $clause .= "6";   /* 6-digit */ }
      else { $clause .= "7";   /* 7-digit */ }

      $clause .= " AND SUBSTRING(" . $table_prefix . "student_number, 1, 2)='" . substr($year, -2) . "'";
   }
   return $clause;
}


function whereClauseFirstName($name, $table_prefix)
{
   if ($table_prefix != "") { $table_prefix .= "."; }
   return (($name != "") ? ($table_prefix . "first_name LIKE '" . addslashes($name) . "%'") : "");
}


function whereClauseLastName($name, $table_prefix)
{
   if ($table_prefix != "") { $table_prefix .= "."; }
   return (($name != "") ? ($table_prefix . "last_name LIKE '" . addslashes($name) . "%'") : "");
}


function whereClauseStudentNumber($student_num, $table_prefix)
{
   if ($table_prefix != "") { $table_prefix .= "."; }
   return (($student_num != "") ? ($table_prefix . "student_number LIKE '%" . addslashes($student_num) . "%'") : "");
}


function whereClause($year, $first_name, $last_name, $student_num, $table_prefix)
{
   $where = whereClauseYear($year, $table_prefix);
   $where1 = whereClauseFirstName($first_name, $table_prefix);
   $where2 = whereClauseLastName($last_name, $table_prefix);
   $where3 = whereClauseStudentNumber($student_num, $table_prefix);
   if ($where1 != "") { $where .= (($where) ? " AND " : " ") . $where1; }
   if ($where2 != "") { $where .= (($where) ? " AND " : " ") . $where2; }
   if ($where3 != "") { $where .= (($where) ? " AND " : " ") . $where3; }
   return $where;
}

?>
