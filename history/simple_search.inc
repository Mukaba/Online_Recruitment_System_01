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
 | Filename: simple_search.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This displays the form in which a user can search for           |
 | student's placement history records.                                         |
 +------------------------------------------------------------------------------+

*/

    if ($userlevel == OFFICE)
    {
        // If we come from the student quickmenu, we hav a record_id for the target student, so we derive the student's academic department id.
        if ($record_id && !$department_id)
        {
            $sql = ("
                SELECT department_id
                FROM student_department
                WHERE record_id = '".$record_id."'
                ");
            $results = $GLOBALS['dbh']->Execute($sql);
            $row = $results->FetchRow();
            $department_id = $row['department_id'];
        }

        if ($department_id == "")
        {
            $sql = "SELECT pulldown_menu_group 
                    FROM contact_internal 
                    WHERE department_id=".$auth->department." AND (login_id='".$login_id."' OR netlink_id='".$login_id."')";
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();	

            if ($row['pulldown_menu_group']==1)
            {
                $department_id = 0;
            }   
            else
            {
                $department_id = $auth->department;
            }
        }

        if ($department_id != "0" && !in_array($department_id, $departments_in_group))
        {
            $error = ("You do not have access to this department");
            $department_id = $auth->department;
            $page = "";
            unset( $PDF );
        }

        if ($department_id == "0")
        {
            $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
        }
        else
        {
            $department_in_str = "'" . $department_id . "'";
        }
    }
    
    // Start displaying 
    echo("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");

    echo("<table width='99%' cellpaddind='2'>");
    echo("<tr>");
    echo("<td><h3 align='left'>Student Information - Placement History</h3></td>\n");

    echo("<td align='right'>");

    if ($btnSubmit=="View Placements")
    {
        // Get record_id for use in quickmenu
        $arr_dept_allowed = department_groups($auth->department);  //find out what departments the user is allowed to view
        foreach ($arr_dept_allowed as $dept_key => $dept_value)    //run through all the values in this array 
        {
            $dept_allowed = $dept_allowed."'".$dept_value."'";     //add current value to string in single quotes 
            if ($dept_value != end($arr_dept_allowed))             //if the current value is not the last value of the array
            {
                $dept_allowed = $dept_allowed.",";                 //then add a comma after the previous single quoted value
            }
        }
    
        $sql_get_record_id = "SELECT DISTINCT record_id
                              FROM student s 
                              INNER JOIN student_department sd  
                              ON s.student_number = sd.student_number
                              WHERE sd.department_id IN (".$dept_allowed.")"; 
                                                                                                   
        //find out what we should search by:
        if ($student_num)                  //if a student was selected from the list
        {
            $sql_get_record_id = $sql_get_record_id." AND s.student_number='".$student_num."'";
        }
        elseif ($student_number_select)    //if a student number was entered in the appropriate text field
        {
            $sql_get_record_id = $sql_get_record_id." AND s.student_number='".$student_number_select."'";
        }
        elseif ($student_name_select)      //if a student's name was entered in the appropriate text field
        {
            $arr_student_name = explode(",",$student_name_select);  //break up string into array containing first and last name
            $first_name = trim($arr_student_name[1]);               //trim off white space and store first name
            $last_name = trim($arr_student_name[0]);                //trim off white space and store last name
            $sql_get_record_id = $sql_get_record_id." 
                AND s.first_name LIKE '".$first_name."%' 
                AND s.last_name LIKE '".$last_name."%'";
        }
    
        //attempt to find this student in the database
        $result_get_record_id = $GLOBALS['dbh']->Execute($sql_get_record_id);
    
        //analyze the results:
        //:TODO: This code is incorrect, it doesn't work in cases of students in multiple departments
        if ($result_get_record_id->RecordCount() >= 1)                           //there was at least one result, so...
        {
            $row_get_record_id = $result_get_record_id->FetchRow();        //fetch the first result
            $record_id = $row_get_record_id['record_id'];                      //store the record_id for comparison
            while($row_get_record_id = $result_get_record_id->FetchRow())  //check the next record
            {  
                $temp_record_id = $row_get_record_id['record_id'];              //and store the next record_id temporarily
                if($record_id != $temp_record_id)                               //if this record_id is not the same as the first 
                {
                   unset($record_id);                                           //then don't display quickmenu
                   break;                                                       //so stop searching (i.e. jump out of the loop)
                }
            }
        } 
        else                     //there were no results
        {
            unset($record_id);   //do not have a record_id (i.e. we do not need to display quickmenu)
        }
        
        // Display the quickmenu
        if ($record_id || $showquick)
        {
            $showquick = 1;
            include('student_info/student_quickmenu.inc');
        }
        
        echo("</td>\n");
        echo("</tr>");
        echo("</table>");    //close the quickmenu's table
         
        include("history/simple_search_results.inc");
    }
  
  else
    {
        echo("</td>\n");
        echo("</tr>");
        echo("</table>");    //close the quickmenu's table

        // Used by the main form's change department button.
        ?>
        <script type="text/javascript" language="javascript">
        <!--javascript

        function triggerGoButtonEvent() {
            document.go_form.department_id.value = document.myform.department_id.value;
            document.go_form.student_number_select.value = document.myform.student_number_select.value;
            document.go_form.student_name_select.value = document.myform.student_name_select.value;
            document.go_form.submit();
        }

        //-->
        </script>

        <?php

        //displays a form to select the department and student form

        $order_href = $PHP_SELF . "&amp;select=history_choose&amp;searchmode=simple&amp;department_id=" . urlencode($department_id);

        global $departments_in_group;
        global $department_id;
        global $department_in_str;
        global $page;
        global $auth;

        if ($order == "")
        {
            $order="A";
        }
        
        echo("<table cellspacing='0' class='row1' width='97%' border='0' cellpadding='8'>");
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<center>");
        echo("<form method='post' action='".$PHP_SELF."&amp;select=history_choose'>");
        echo("<input type='hidden' name='searchmode' value='advanced' />");
        echo("<input type='submit' value='Go to Advanced Search' />");
        echo("</form>");
        echo("</center>");
        echo("</td>");
        echo("</tr>\n");
        echo("</table>");
        
        echo("<form method='post' name='myform' action='".$PHP_SELF."&amp;select=history_choose&amp;searchmode=simple'>");
        echo("<table cellspacing='0' class='row1' width='97%' border='0' cellpadding='8'>");
        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<input type='hidden' name='search_by_student' value='true' />");
        echo("<b class='black'>");
        echo("Please enter a student number, name, or choose a student from the list <br />");
        echo("below, and select the term(s) for which you wish to view history records.<br />");
        echo("  If entering a student's name, it must be done in the format <i>Last Name, First Name.</i><br />");
        echo("  You may enter only the beginning of either the first or last name, but they must be separated by a comma.<br />");
        echo("</b>");
        echo("</td>");
        echo("</tr>\n");
        ?>

            <tr>
            <td width="30%" align="right">Student Number:</td>
            <td align="left"><input type="text" size="10" name="student_number_select" value="<?php echo(stripslashes(htmlentities($student_number_select, ENT_QUOTES))); ?>" /></td>
            </tr>


            <tr>
            <td align="right">Student Name:</td>
            <td align="left"><input type="text" size="30" name="student_name_select" value="<?php echo(stripslashes(htmlentities($student_name_select, ENT_QUOTES))); ?>" /></td>
            </tr>
            
            <tr>
            <td colspan="2" align="center">
            <?php
            //:TODO: This query is inefficient. It's pulling out thousands of student records just to determine which letters we
            //       should display based on last names. 
            $letter_sql = ("
                SELECT distinct SUBSTRING(s.last_name, 1, 1) AS order_letter
                FROM student as s ,student_department as smd,student_flags_join as sfj
                WHERE s.student_number=smd.student_number
                AND sfj.record_id=smd.record_id
                AND sfj.student_flags_id=".CURRENT_FLAG."
                AND smd.department_id IN (".$department_in_str.")
                ORDER BY s.last_name
                ");
        $letter_results=$GLOBALS['dbh']->Execute($letter_sql);
        
        while ($letter_row=$letter_results->FetchRow()){
            if (!$firstletter){
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
            WHERE s.last_name LIKE '" . $order . "%' AND sfj.record_id=smd.record_id AND sfj.student_flags_id=".CURRENT_FLAG.
            " AND (smd.department_id IN (".$department_in_str."))
            ORDER BY s.last_name, s.first_name";
        $result = $GLOBALS['dbh']->Execute($sql);

        $letter_results=$GLOBALS['dbh']->Execute($letter_sql);
        
        // $x is used to determine when to prepend a letter with a bar | 
        $x = 0;
        while ($letter_row=$letter_results->FetchRow()){
            if (strtolower($lastLetter)<>strtolower($letter_row['order_letter'])){
                if ($x)
                {
                    echo(" | ");
                }
                ?>
                    <a class = "blue" href="<?php echo ($order_href) ?>&amp;order=<?php echo $letter_row['order_letter']?>">
                    <?php echo strtoupper($letter_row['order_letter'])?></a> 
                    <?php
                $x++;
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

            // hack to prevent browser from automatically choosing the first student in the select box when we have one student
            // returned from the query 
            if ($size == 1) {
                $size++;
            }
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
                    <tr>
                    <td align="right">Department:</td>
                    <td><select name="department_id">
                    <option value="0" <?php echo ($department_id==0) ? "selected='selected'" : ""; ?>>All in Group</option>
                    <?php
                    foreach($departments_in_group as $dep){
                        echo "\t<option value='$dep'" . (($dep == $department_id) ? "selected='selected'" : "" ) . ">" . getDepartmentName($dep)  . "</option>\n";
                    }
                ?>
                    </select>
                    &nbsp;&nbsp;<input type="button" value="Go" onclick="javascript:triggerGoButtonEvent();" /></td>
                    </tr>
                    <tr><td colspan="2" align="center">
                    Note: students with an asterisk (*) are in your department.
                    </td>
                    </tr>
                    <?php
            }

        $sql = ("
            SELECT MIN(year)
            FROM history
            WHERE year != 0
            ");
        $start_year = $GLOBALS['dbh']->GetOne($sql);

        $sql = ("
            SELECT term_id, term_name
            FROM term
            ORDER BY year_order
            ");
        $results = $GLOBALS['dbh']->Execute($sql);
        $num_terms = $results->RecordCount();
        if (!is_null($start_year)){
            for ($i=1;$i<=$num_terms;$i++)
            {
                echo("<tr>");
                    echo("<td class='row1' colspan='2' align='center'>");
                    echo("<center>");
                    echo("<input type='checkbox' name='termsused[".$i."]' value='".$i."' />");
                    echo("<select name='term[".$i."]'>");
                    
                    // Display terms for drop down menu. 
                    while ($term_row = $results->FetchRow())
                    {
                        if ($i==$term_row['term_id'])
                        {
                            $selected="selected='selected'";
                        }
                        echo("<option value='".$term_row['term_id']."' ". $selected.">".$term_row['term_name']."</option>");
                        $selected=NULL;

                    } 
                    $results->Move(0);
                    echo("</select>");

                    echo("<select name='year[".$i."]'>");

                    $this_year=date("Y");
                    for ($year=$this_year + 1;$year>=(int)$start_year;$year--)
                    {
                        if ($year == $this_year)
                        {
                            $selected = "selected='selected'";
                        }
                        echo ("<option value='".$year."' ".$selected.">".$year."</option>");
                        $selected = NULL;
                    }
                    echo("</select>");
                    echo("</center>");
                    echo("</td>");
                echo("</tr>");
            }
        }
        echo("<tr>");
        echo("<td class='row1' colspan='2' align='center'><center><input type='checkbox' name='all' value='true' />All Terms</center></td>");
        echo("</tr>");

        echo("<tr>");
        
        echo("<td class='row1' colspan='2' align='center'><hr /><input type='submit' name='btnSubmit' value='View Placements' />&nbsp;&nbsp;&nbsp;<input type='reset' value='Reset Form' />");
        echo("<input type='hidden' name='gender' value='A' />");
        echo("<input type='hidden' name='search_by_term' value='true' />");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
       echo("</form>");
       echo("<form method='post' name='go_form' action='" . $PHP_SELF . "'>");
           echo("<input type='hidden' name='select' value='history_choose' />");
           echo("<input type='hidden' name='searchmode' value='simple' />");
           echo("<input type='hidden' name='department_id' value='' />");
           echo("<input type='hidden' name='student_number_select' value='' />");
           echo("<input type='hidden' name='student_name_select' value='' />");
       echo("</form>");
    }
?>
