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
 | Filename: view_main.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: Allows the user to enter a student number/name and view all     |
 | the coverletters that correspond to that student                             |
 +------------------------------------------------------------------------------+

*/
?>
<script type="text/javascript" language="javascript">
<!--javascript

function triggerGoButtonEvent() {
    document.go_form.department_id.value = document.myform.department_id.value;
    document.go_form.student_num.value = document.myform.student_num.value;
    document.go_form.student_name.value = document.myform.student_name.value;
    document.go_form.submit();
}

//-->
</script>

<h3>Student Information - Cover Letters</h3>

<form method="post" action="<?php echo ($PHP_SELF)?>" name="myform">
<input type="hidden" name="select" value="coverletter" />

<?php
$order_href = $PHP_SELF."&amp;select=coverletter&amp;page=view";

    echo("<table width='100%' border='0' cellspacing='0' cellpadding='8' class='row1'>");
    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>");
    echo("Please enter a student number, name, or choose a student from the list below.<br />");
    echo("  If entering a student's name, it must be done in the format <i>Last Name, First Name.</i><br />");
    echo("  You may enter only the beginning of either the first or last name, but they must be separated by a comma.");
    echo("</b>");
    echo("</td>");
    echo("</tr>\n");
    echo("<tr>");
    echo("<td align='right'>");
    echo("Student Number:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' value='" . stripslashes(htmlentities($student_num, ENT_QUOTES)) . "' size='10' name='student_num' />");
    echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td align='right'>");
    echo("Student Name:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' value='" . stripslashes(htmlentities($student_name, ENT_QUOTES)) . "' size='30' name='student_name' />");
    echo("</td>");
    echo("</tr>\n");

	// Creates a list from A-Z that represents the student's last names. If there are no last names for a certain letter, 
    // then that letter is omitted.
	// If a student doesn't have a coverletter, the he/she is also omitted from the list. 
	$sql = "select distinct substring(s.last_name, 1, 1) as order_letter 
        	from student as s, student_department as sd, student_flags_join as sf, coverletter as c 
          	where sd.department_id IN (".$department_in_str.") and sd.record_id = sf.record_id and sf.student_flags_id = '7'
            	      and sd.student_number=s.student_number and sd.student_number = c.student_number and s.last_name != ''
          	order by s.last_name";
 	$response = $GLOBALS['dbh']->Execute($sql);

    while ($row = $response->FetchRow())
    {
        if (!is_array($order_choices) || !in_array(strtoupper($row["order_letter"]), $order_choices))
        {
            $order_choices[] = strtoupper($row["order_letter"]);
        }
    }

    if (!$order)
    {
        $order = $order_choices[0];
    }

    //echo("<input type='hidden' name='order' value='" . $order . "' />");

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<input type='hidden' name='order' value='" . $order . "' />");
    // Display the order hrefs.
    if($department_id == 0)
    {
        $order_href = ($PHP_SELF . "&amp;select=coverletter&amp;page=view&amp;department_id=0");
    }
    elseif (in_array($department_id, $departments_in_group))
    {
        $order_href = ($PHP_SELF . "&amp;select=coverletter&amp;page=view&amp;department_id=" . $department_id);
    }
    else
    {
        $order_href = ($PHP_SELF . "&amp;select=coverletter&amp;page=view&amp;department_id=" . $auth->department);
    }

    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>");
        echo($order_choices[$i]);
        echo("</a> | ");
    }

    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>");
    echo($order_choices[$i]);
    echo("</a>");

    echo("</td>");
    echo("</tr>\n");

	// Brings back a list of students with a last name that starts with $order. 
    // The students must be current, and also have a coverletter in the database.
  	$sql = "SELECT DISTINCT s.student_number, sd.department_id, CONCAT(s.first_name, ' ',s.last_name) as name
		FROM student as s, student_department as sd, student_flags_join as sf, coverletter as c 
		WHERE s.last_name LIKE '" . $order . "%' AND sd.department_id IN (".$department_in_str.") AND sd.student_number = c.student_number
		      AND sd.student_number=s.student_number AND sd.record_id = sf.record_id AND sf.student_flags_id = '7'
		ORDER BY s.last_name,s.first_name";

	$result = $GLOBALS['dbh']->Execute("$sql");

    echo("<tr>");
    echo("<td class='row1' colspan='2'  align='center'>");

        if (!$result->RecordCount())
        {
            echo("<tr>");
            echo("<td colspan='2' align='center'>");
            echo("There are no coverletters for students in your department with last names that start with the chosen letter.");
            echo("</td>");
            echo("</tr>\n");
        }
		else
		{
			$size=10;
			if ($result->RecordCount() < 10){
				$size = $result->RecordCount();

                // hack to prevent browser from automatically choosing the first student in the select box when we have one student
                // returned from the query 
                if ($size == 1) {
                    $size++;
                }
			}

			echo("<select name='student_num2' size='".$size."'>");
			$lastStudentNumber=0;
			while ($row = $result->FetchRow()){
                echo("<option value='" . $row["student_number"] . "'>");
                echo(($row["department_id"] == $auth->department && sizeof($departments_in_group) > 1) ? "*" : "");
                echo("[" . $row["student_number"] . "] " . $row["name"] . "</option>");
			}
            echo("</select>");
        }
    echo("</td>");
    echo("</tr>");

    if (sizeof($departments_in_group) > 1)
    {
        echo("<tr>");
        echo("<td colspan='2' align='center'>Show students for:&nbsp;");
        echo("<select name='department_id'>");

        if (sizeof($departments_in_group) > 1)
        {
            echo("<option value='0' ");
            echo(($department_id == 0) ? "selected='selected'" : "");
            echo(">Your Co-op Group</option>");
        }

        foreach($departments_in_group as $dig)
        {
            echo("<option value='" . $dig . "' ");
            echo(($department_id == $dig) ? "selected='selected'" : "");
            echo(">" . getDepartmentName($dig) . "</option>");
        }

        echo("</select>");
        echo("&nbsp;&nbsp;");
        echo("<input type='button' value='Go' onclick='javascript:triggerGoButtonEvent();' />");
        echo("</td>");
        echo("</tr>\n");
    }
	  if (sizeof($departments_in_group)>1) {
          echo("<tr><td colspan='2' align='center'>Note: students with an asterisk (*) are in your department.</td></tr>");
	  }
?>
  <tr><td class="row1" colspan="2"><hr width="95%" /></td></tr>
  <tr>
    <td colspan="2" align="center">
      <input type="submit" name="page" value="View Cover Letters" />
      &nbsp;&nbsp;&nbsp;
      <input type='reset' name='reset' value='Reset Form' />
    </td>
  </tr>
</table>
</form>
<form method="post" action="<?php echo ($PHP_SELF)?>" name="go_form">
<input type="hidden" name="select" value="coverletter" />
<input type="hidden" name="department_id" value="" />
<input type="hidden" name="student_num" value="" />
<input type="hidden" name="student_name" value="" />
<input type="hidden" name="continue" value="Go" />
</form>
