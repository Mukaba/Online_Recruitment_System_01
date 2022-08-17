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
 | Filename: choose_student.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the students matching the user's         |
 | criteria from view_main.inc                                                  |
 +------------------------------------------------------------------------------+

*/

$show = 1;
$errorset = 0;

/*
	Search for Students...
	Variables sent by view_main.inc are : first_name, last_name, student_num
*/
    if ($department_id == '0' || $department_id == '') {
            $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
            $department_in_str = "(" . $department_in_str . ")";
    } else {
            $department_in_str = "('$department_id')";
    }

if ($errorset == '0')
{
	$sql = "SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as student_name, 
                d.department_name,
                DATE_FORMAT(t.last_modified, '%b %d, %Y') as last_modified 
                FROM student as s 
				LEFT JOIN student_department as sd on sd.student_number=s.student_number
                INNER JOIN transcript as t on t.student_number=s.student_number
                LEFT JOIN department as d on d.department_id=sd.department_id
			WHERE
			s.first_name like '".$first_name."%' AND
			s.last_name like '".$last_name."%' AND
            sd.department_id IN ".$department_in_str."
			ORDER BY s.last_name, s.first_name";

		$get_transcripts = $GLOBALS['dbh']->Execute($sql);

            echo("<table width=100%>");
            echo("<tr>");
            echo("<td><h3 align='left'>Student Information - Transcripts</h3></td>\n");

            // Display the quickmenu
            echo("<td align='right'>");
            if ($record_id && ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR))
            {
                include('student_info/student_quickmenu.inc');
            }
            echo("</td>\n");
            echo("</tr>");
            echo("</table>");
            instruct("Please choose a specific student:");
?>
        <form method="post" action="<?php echo( $PHP_SELF )?>">	
			<table border="1" cellspacing="0" cellpadding="0"><tr><td>
  			  <table border="0" cellpadding="2">
			  <tr>
			    <td class="rowgrey" align='center'><b class="white">&nbsp;</b></td>
			    <td class="rowgrey" align='center'><b class="white">Student Name</b></td>
			    <td class="rowgrey" align='center'><b class="white">Student Number</b></td>
			    <td class="rowgrey" align='center'><b class="white">Department</b></td>
			    <td class="rowgrey" align='center'><b class="white">Last Modifed</b></td>
			  </tr>
<?php

			$numrows = $get_transcripts->RecordCount();
			$maxrows = 50;  // Maximum number of students to display before telling the user to be more specific in his search.
			while (($row = $get_transcripts->FetchRow()) && ($j < $maxrows)) {
					$class = ($j%2 == 0) ? "row1d" : "row0d";
					$j++;
						
					echo("<tr>");
					echo("<td class='".$class."' align='center' nowrap='nowrap'><input type='radio' name='student_num' value='".$row["student_number"]."' /></td>");
					echo("<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["student_name"]."</td>");
					echo("<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["student_number"]."</td>");
					echo("<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["department_name"]."</td>");
					echo("<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["last_modified"]."</td></tr>");
			}

			if ($j == 0) {
				echo 	"<tr>
						<td class='row0d' colspan='5' align='center' valign='middle'><br />
							No Matches For Your Search Criteria, please <a href='$PHP_SELF&amp;select=transcript&amp;page=view'>try again</a>.<br />&nbsp;	
						</td>
					</tr>";
			} elseif ($j >= $maxrows) {
				echo 	"<tr>
						<td class='row0d' colspan='5' align='center' valign='middle'><br />
							Too Many Matches For Your Search Criteria (". $numrows . "), please <a href='$PHP_SELF&amp;select=transcript&amp;page=view'>try a more specific search</a>.<br />&nbsp;	
						</td>
					</tr>";
			} 
?>

			</table>
		</td>
	</tr>
</table>

<br />
<input type="hidden" value="<?php echo $department_id?>" name="department_id" />
<input type="hidden" value="<?php echo $student_num?>" name="student_num_passthru" />
<input type="hidden" value="1" name="PDF" />
<input type="hidden" value="transcript" name="select" />
<input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
<input type="hidden" value="<?php echo($new_page)?>" name="new_page" />
<input type="hidden" value="<?php echo($first_name)?>" name="first_name" />
<input type="hidden" value="<?php echo($last_name)?>" name="last_name" />

<?php
if ($j != 0)
{
    echo("<table class='row1' cellspacing='0' cellpadding='5' border='0'><tr><td>");
    echo("<input type='submit' value='View HTML' name='page' /> &nbsp;&nbsp;");
    echo("<input type='submit' value='View PDF' name='page' />&nbsp;&nbsp;");
    echo("</td></tr></table>");
}
echo("</form>");
}
?>
