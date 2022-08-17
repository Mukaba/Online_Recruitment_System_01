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
 | Filename: company_department.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: What does this file do?  It's a mystery!  It seems to let you   |
 | choose a company and department to view history records for.                 |
 +------------------------------------------------------------------------------+

*/
	if ($company_name <>NULL){
		$sql = "select ec.company_name, ec.employer_id, ed.department_name, ed.city,ed.department_id
			from employer_company as ec 
			left join employer_department as ed
			on ed.employer_id=ec.employer_id
			where ec.company_name like '%".$company_name."%'
			and ec.company_display = 1
			order by ec.company_name,ed.department_name";
		if ($division_name <> NULL){
			$sql = "select ec.company_name, ec.employer_id, ed.city,ed.department_name, ed.department_id
				from employer_company as ec 
				left join employer_department as ed
				on ed.employer_id=ec.employer_id
				where ec.company_name like '%".$company_name."%'
				and ed.department_name like '%".$division_name."%'
				and ec.company_display = 1
				order by ec.company_name,ed.department_name";
		}
	}else{
		if ($division_name<>NULL){
			$sql = "select ec.company_name, ec.employer_id, ed.department_name, ed.city,ed.department_id
				from employer_company as ec 
				left join employer_department as ed
				on ed.employer_id=ec.employer_id
				where ed.department_name like '%".$division_name."%'
				and ec.company_display = 1
				order by ec.company_name,ed.department_name";
		}else{
			include('history/employer/results.inc');
			$sql=NULL;
		}
	}

	if ($sql<>NULL){
		$results = $GLOBALS['dbh']->Execute($sql); 
		if ($results->RecordCount()!=0){
			if ($results->RecordCount()>1){
            $flag = 0;
            while ($row=$results->FetchRow()){
                while ($row=$results->FetchRow()){
                    if ($lastcompany<>$row['employer_id'] && $row['department_id']<>NULL){
                        $flag++;
                    }
					$lastcompany=$row['employer_id'];
                }
            }
                
?>
				<h3>Employer Information - Hiring History</h3>
				<h4>Choose a Division:</h4>
				<form method='post' action='<?php echo $PHP_SELF?>'>
				<input type='hidden' name='select' value='employer_history' />
				<input type='hidden' name='term_id' value='<?php echo $term_id?>' />
				<input type='hidden' name='year' value='<?php echo $year?>' />
				<input type='hidden' name='departments' value='<?php echo $departments?>' />
				<table cellpadding='0' border='1' cellspacing='0'><tr><td>
				<table cellpadding='2' border='0'>
				
					<tr>
						<td align='center' class='rowgrey'>&nbsp;</td>
						<td align='center' class='rowgrey'><b class='white'>&nbsp;Company&nbsp;</b></td>
						<td align='center' class='rowgrey'><b class='white'>&nbsp;Division&nbsp;</b></td>
					</tr>
<?php
				$count = 0;
				$checked = "checked='checked'";
				$rowclass=1;
                $results = $GLOBALS['dbh']->Execute($sql);
				while ($row=$results->FetchRow()){
					$count=$count+1;
					if ($row['city']<>NULL){
						$city="(".$row['city'].")";
					}else{
						$city=NULL;
					}
					if ($lastCompany<>$row['employer_id'] && $row['department_id']<>NULL){
                        if($flag > 1)
                        {
                            $checked = NULL;
                        }
                        else
                        {
                            $checked = "checked='checked'";
                        }
?>
						<tr class='row2d'>
							<td align='center'><input <?php echo $checked?> type='checkbox' name='company_department[<?php echo $count?>]' value='<?php echo $row['employer_id']?>' /> </td>
						<td align='center'>&nbsp;<?php echo $row['company_name']?>&nbsp;</td>
						<td align='center'><b class='black'>&nbsp;All Divisions&nbsp;</b></td>
						</tr>
<?php
                        $checked = NULL;
					}
?>
					<tr class='<?php echo(($rowclass % 2) ? "row0d" : "row1d"); ?>'>
						<td align='center'>&nbsp;<input type='checkbox' <?php echo $checked?> name='company_department[<?php echo $count?>]' value='<?php echo $row['employer_id'].":".$row['department_id']?>:' />&nbsp;</td>
						<td align='center'>&nbsp;<?php echo $row['company_name']?>&nbsp;</td>
						<td align='center'>&nbsp;<?php echo $row['department_name']." ".$city?>&nbsp;</td>
					</tr>
<?php
					$checked = NULL;
					$lastCompany=$row['employer_id'];
					$rowclass++;
				}
?>
					<tr class='<?php echo(($rowclass % 2) ? "row0d" : "row1d"); ?>'>
						<td colspan='3' align='center'><input type='Submit' name='Submit' value='View' /></td>
					</tr>
				</table>
				</td></tr></table>
				</form>
<?php
			}else{
				$row=$results->FetchRow();
				$employer_id=$row['employer_id'];
                if($division_name){
                    $department_id = $row['department_id'];
                }
			    include ('history/employer/results.inc');
			}
		}else{
			notify("The company you specified was not found");
		}
	}
?>
