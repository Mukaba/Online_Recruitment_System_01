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
 | Description: Employer information related to hiring history.                 |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Employer Information - Hiring History</h3>
<form name='myform' action='<?php echo $PHP_SELF?>' method='post'>
<table class='row1' cellpadding='10' cellspacing='0' border='0'>
	<tr>
		<td>Company:</td>
		<td><input type='text' size='50' name='company_name' id='company_name1' />&nbsp;&nbsp;<input type='button' onclick='popUpChooser("company_name1", "",  "division_name1", "");' value='Select From List' /></td>
	</tr>
	<tr>
		<td>Division:</td>
		<td><input type='text' size='50' name='division_name' id='division_name1'/></td>
	</tr>
	<tr>
		<td>Department:</td>
		<td>
			<select name='departments'>
<?php
            //if $department_id is not set, then set it to the users dept and access the default settings (either GROUP or DEPT) of their drop-down menus for 
            //proper drop-down menu display later
            if($department_id == "")
            {
                $sql = ("
                    SELECT pulldown_menu_group 
                    FROM contact_internal 
                    WHERE department_id=".$auth->department." AND (login_id='".$login_id."' OR netlink_id='".$login_id."')
                    ");
    
                $results=$GLOBALS['dbh']->Execute($sql);
                $row=$results->FetchRow();
            
                if ($row['pulldown_menu_group'] == 1)
                {
                    //GROUP default drop-down menu  
                    $department_id = 0;
                }
                else
                {
                    //DEPT default drop-down menu
                    $department_id = $auth->department;
                }
            }

			$departments_in_str = implode("','",$departments_in_group);
			$departments_in_str = "('".$departments_in_str."')";

			$department_id=pulldownGroup($departments_in_group);
			echo("<option value='uvic'>All at ".ORGANIZATION_CODE."</option>");
            if(sizeof($departments_in_group)>1)
            {
			    echo("<option value='group' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
			}
            $department_sql = "Select department_name,department_id from department where department_id in ".$departments_in_str;
			$department_results = $GLOBALS['dbh']->Execute($department_sql);
			$selected=NULL;
			while ($department_row=$department_results->FetchRow()){
				if ($department_row['department_id']==$department_id){
					$selected="selected='selected'";
				}
?>
				<option <?php echo $selected?>  value='<?php echo $department_row['department_id']?>'><?php echo $department_row['department_name']?></option> 
<?php
				$selected=NULL;
			}
?>

			</select>
		</td>
	</tr>
	
	<tr>
		<td>Term:</td>
		<td>
			<select name='term_id'>
				<option value=''>&nbsp;</option>
<?php
				$sql = "select term_id,term_name from term";
				$results = $GLOBALS['dbh']->Execute($sql);
				while ($row=$results->FetchRow()){
?>
					<option value='<?php echo $row['term_id']?>'><?php echo $row['term_name']?></option>
<?php
				}
?>
			</select>
			<select name='year'>
				<option value=''>&nbsp;</option>
<?php
                $year_sql = ("
                        SELECT MIN(year) AS min_year,
                        MAX(year) AS max_year
                        FROM history
                        WHERE year != 0
                        ");
                $year_results = $GLOBALS['dbh']->Execute($year_sql);
                $year_row = $year_results->FetchRow();
                $max_year = $year_row['max_year'];
                $min_year = $year_row['min_year'];
                for ($i=(int)$max_year; $i>=(int)$min_year;$i--)
                {
?>
					<option value='<?php echo($i); ?>'><?php echo($i); ?></option>
<?php
				}
?>
			</select>
		</td>
	</tr>
	<tr>
		<td colspan='2'><center><hr />
        <input type='submit' name=" Submit" value='Continue' />
        <input type='hidden' name='select' value='employer_history' />
        </center></td>
	</tr>
</table>
</form>
