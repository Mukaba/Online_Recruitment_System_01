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
 | Filename: search.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: Search for a job or student selected by the user (see           |
 | search_main.inc).                                                            |
 +------------------------------------------------------------------------------+

*/

if ($page == "job search"){
	if ($job_code != ""){
		$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company
			FROM job_info as i, employer_company as e
			WHERE i.employer_id = e.employer_id AND i.job_code LIKE '%" . addslashes(stripslashes($job_code)) . "%'
			 	AND i.department_id IN (" . $department_in_str . ")";
	}elseif($company_name != ""){
		$sql = "SELECT i.job_id as job_id, i.job_code as job_code, e.company_name as company
			FROM job_info as i, employer_company as e
			WHERE i.employer_id = e.employer_id AND e.company_name LIKE '%" . addslashes(stripslashes($company_name)) . "%'
				AND i.department_id IN (" . $department_in_str . ")";
		
	}
	if ($sql != ""){
		$result = $GLOBALS['dbh']->Execute( $sql );
		?>
		<h3>Query Results</h3>
		<?php
		if ($result->RecordCount() <= 0){
			?>
			<b class="black">No matches.</b>
		<?php
		}else{
		?>
			<table border="1">
			  <tr>
			    <td class="rowgrey"><b class="white">Job Code</b></td>
			    <td class="rowgrey"><b class="white">Company</b></td>
			  </tr>
			<?php
			$i = 0;
			while ($row =  $result->FetchRow()){
				$class = ($i % 2 == 0) ? "row0d" : "row1d";
				$i ++;
				$href = $PHP_SELF . "&amp;select=coverletter&amp;page=viewletters&amp;department_id=" . urlencode($department_id) . "&amp;job_id=" . $row["job_id"];
				?>
				<tr>
				  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo $row["job_code"]?></a></td>
				  <td class="<?php echo $class?>"><a class="blue" href="<?php echo $href?>"><?php echo $row["company"]?></a></td>
				</tr>
				<?php
			}
			?>
			</table>
			<?php
		}
	}

}else{
	//search for students
	?>
	<h3>Student Search</h3>
	<form method="post" action="<?php echo $PHP_SELF?>">
	<input type="hidden" name="select" value="coverletter" />
	<input type="hidden" name="page" value="student_search" />
	<input type="hidden" name="order" value="<?php echo $order?>" />
	<table border="0" cellspacing="3" width="100%">
	  <tr>
	    <td colspan="4"><hr width="100%"></td>
	  </tr>
	  <?php
	  if (sizeof($departments_in_group) > 1){
		  ?>
		  <tr>
		    <td>Department:</td>
		    <td colspan="3"><select name="department_id">
		    		    <option value="0">All</option>
				    <?php
				    foreach($departments_in_group as $dep){
					echo "<option value='" . $dep . "'" . (($dep == $department_id) ? "selected='selected'" : "") . ">" . getDepartmentName($dep) . "</option>";
				    }
				    ?>
				    </select>
		    </td>
		  </tr>
		  <?php
	  }
	  ?>
	  <tr>
	    <td>First Name:</td>
	    <td><input type="text" name="first_name" value="<?php echo $first_name?>" size="20" /></td>
	    <td>Student Number:</td>
	    <td><input type="text" name="stud_number" value="<?php echo $stud_number?>" size="10" /></td>
	  </tr>
	  <tr>
	    <td>Last Name:</td>
	    <td><input type="text" name="last_name" value="<?php echo $last_name?>" size="20" /></td>
	    <td colspan="2">&nbsp;</td>
	  </tr>
	  <tr>
	    <td colspan="4" align="center"><input type="submit" value="Search" /><hr width="100%"></td>
	  </tr>
	<?php
	$href = $PHP_SELF . "&amp;select=coverletter&amp;page=student_search&amp;department_id=" . urlencode($department_id) . "&amp;order=";
	?>
	  <tr>
	    <td colspan="4" align="center">
	    	<a class="blue" href="<?php echo $href . "A"?>">A</a> | 
	    	<a class="blue" href="<?php echo $href . "B"?>">B</a> | 
	    	<a class="blue" href="<?php echo $href . "C"?>">C</a> | 
	    	<a class="blue" href="<?php echo $href . "D"?>">D</a> | 
	    	<a class="blue" href="<?php echo $href . "E"?>">E</a> | 
	    	<a class="blue" href="<?php echo $href . "F"?>">F</a> | 
	    	<a class="blue" href="<?php echo $href . "G"?>">G</a> | 
	    	<a class="blue" href="<?php echo $href . "H"?>">H</a> | 
	    	<a class="blue" href="<?php echo $href . "I"?>">I</a> | 
	    	<a class="blue" href="<?php echo $href . "J"?>">J</a> | 
	    	<a class="blue" href="<?php echo $href . "K"?>">K</a> | 
	    	<a class="blue" href="<?php echo $href . "L"?>">L</a> | 
	    	<a class="blue" href="<?php echo $href . "M"?>">M</a> | 
	    	<a class="blue" href="<?php echo $href . "N"?>">N</a> | 
	    	<a class="blue" href="<?php echo $href . "O"?>">O</a> | 
	    	<a class="blue" href="<?php echo $href . "P"?>">P</a> | 
	    	<a class="blue" href="<?php echo $href . "Q"?>">Q</a> | 
	    	<a class="blue" href="<?php echo $href . "R"?>">R</a> | 
	    	<a class="blue" href="<?php echo $href . "S"?>">S</a> | 
	    	<a class="blue" href="<?php echo $href . "T"?>">T</a> | 
	    	<a class="blue" href="<?php echo $href . "U"?>">U</a> | 
	    	<a class="blue" href="<?php echo $href . "V"?>">V</a> | 
	    	<a class="blue" href="<?php echo $href . "W"?>">W</a> | 
	    	<a class="blue" href="<?php echo $href . "X"?>">X</a> | 
	    	<a class="blue" href="<?php echo $href . "Y"?>">Y</a> | 
	    	<a class="blue" href="<?php echo $href . "Z"?>">Z</a> 
	    </td>
	  </tr>
	  <tr>
	    <td colspan="4" align="center">
	    <?php
	    $sql = "SELECT student_number, CONCAT(first_name, ' ', last_name) as name
	    		FROM student
			WHERE department_id IN (" . $department_in_str . ") AND";
			
	    if ($stud_number != ""){
		$sql .= " student_number LIKE '%" . addslashes(stripslashes($stud_number)) . "%' AND";
	    }
	    if ($first_name != ""){
		$sql .= " first_name LIKE '%" . addslashes(stripslashes($first_name)) . "%' AND";
	    }
	    if ($last_name != ""){
		$sql .= " last_name LIKE '%" . addslashes(stripslashes($last_name)) . "%' AND";
	    }
	    if ($stud_number == "" && $first_name == "" && $last_name == ""){
		    if ($order=="")  $order="A";
		    $sql .= " last_name LIKE '" . addslashes(stripslashes($order)) . "%' AND";
	    }
	    
	    $sql = substr($sql, 0, strlen($sql) - 4);
	    $sql .= " ORDER BY last_name, first_name, student_number"; 
	    
	    $result = $GLOBALS['dbh']->Execute($sql);
	    
	    if ($result->RecordCount() <= 0 ){
		    ?>
		    <b class="black">No Matches</b>
		    <?php
	    }else{
	    	    ($result->RecordCount() >= 10) ? $size = 10 : $size = $result->RecordCount();
		    ?>
		    <select name="student_num" size="<?php echo $size?>">
		    <?php
		    while ($row = $result->FetchRow()){
			    ?>
			    <option value="<?php echo $row["student_number"]?>">[<?php echo $row["student_number"]?>] <?php echo $row["name"]?></option>
			    <?php
		    }
		    ?>
		    </select>
	    <?php
	    }
	    ?>		    
		    
	    </td>
	  </tr>
	  <tr>
	    <td align="center" colspan="4"><input type="submit" name="page" value="View Cover Letters" /><hr width="100%"></td>
	  </tr>
	</table>
	</form>	
		 










<?php

}
?>
