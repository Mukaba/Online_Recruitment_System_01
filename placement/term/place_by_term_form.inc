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
 | Filename: place_by_term_form.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: Present the user with the menu allowing them to place           |
 | students by term.                                                            |
 +------------------------------------------------------------------------------+

*/


?>
<h3>Job Offers - Place Students by Term</h3>
<form action="<?php echo $PHP_SELF?>&amp;select=placement_by_term" method="post">
<input type='hidden' name='level' value='term_results' />
<table cellpadding='5' cellspacing='0' border='0'>
<?php
	$sql = ("
        SELECT DISTINCT CONCAT(t.term_name,\" \",ji.year) AS term, t.term_id 
        FROM job_info AS ji
        INNER JOIN term AS t 
        ON t.term_id = ji.term_id
        WHERE ji.status != '".CANCELLED."' AND ji.filled != '1' 
        ORDER BY ji.year DESC, t.year_order DESC
        ");
?>
	<tr class='row1'>
		<td align='right'>Term:</td>
        <td>
			<select name='term_id'>
<?php
				$results=$GLOBALS['dbh']->Execute($sql);	
				$select = NULL;
				$next_term = getNextTermYear();
				while($row=$results->FetchRow()){
					$arrTerm=explode(" ",$row['term']);
					if ($row['term_id']==$next_term['term'] && $arrTerm[1]==$next_term['year']){
						$selected = "selected='selected'";
					}
?>
					<option <?php echo $selected?> value='<?php echo $row['term_id'].",".$arrTerm[1]?>'><?php echo $row['term']?></option>
<?php
					$selected=NULL;
				}
?>
			</select>
		</td>
	</tr>
<?php
	if (sizeof($departments_in_group) > 1) {
?>
		<tr class='row1'>
			<td align='right'>Department:</td>
            <td>
				<select name='department_option'>
					<option value='department'>Match your department</option>
<?php
					$department_id=pulldownGroup($departments_in_group);
					echo("<option value='group' ". (($department_id=='0') ? "selected='selected'" : "").">All in Group</option>");
?>
				</select>
			</td>
		</tr>
<?php
	}
?>
	<tr class='row1'>
		<td align='right'>Jobs:</td>
        <td>
			<select name='job_setting'>
				<option value='unmatched'>Show Unmatched Jobs Only</option>
				<option value='all'>Show All Jobs</option>
			</select>
		</td>
	</tr>
	<tr class='row1'>
		<td align='right'>Students:</td>
        <td>
			<select name='student_setting'>
				<option value='eligible'>Show Eligible Students Only</option>
				<option <?php echo $selected?> value='all'>Show All Students</option>
			</select>
		</td>
	</tr>
	<tr class='row1' align="center">
		<td colspan='2'>
            <hr />
            <input type='submit' name='btnSubmit' value='Continue' />
		</td>
	</tr>
</table> 
</form>
