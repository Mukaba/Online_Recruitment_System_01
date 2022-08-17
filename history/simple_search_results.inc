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
 | Filename: simple_search_results.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file returns placement history records from a simple       |
 | search.                                                                      |
 +------------------------------------------------------------------------------+

*/

    include ('history/functions.inc');
    include_once('container_functions.inc');
    include ('history/SearchHistory.class');

    // TODO: a hack to make this search work via quickmenus. We've got naming problems
    // with the quickmen hidden variables, where department_id implies a company's
    // division id, rather than an academic department id. This will have to be revisited
    if ($student_department)
    {
        $department_id = $student_department;
    }
    $grouping = $department_id;
    
	if ($limit=='true')
    {
		$termsused=unpackObject(($termsused));
		$term=unpackObject(($term));
		$year=unpackObject(($year));
	}
    
    // User comes in via the quickmenu to view all history records of a particular student.
    // We blow away the searchHistory object as we do not want any residual search parameters
    // affecting the search. Coming in, the quickmenu provides student_num which is enough info 
    if ($select == 'student_history') {
        $searchHistory = null;
        $search_by_term = true;
        $search_by_student = true; 
    }

    // rebuild the class if it exists already (to re-include the functions),
    // if not, create the class and set the variables.
    if ($searchHistory && is_string($searchHistory)) 
    {
        $searchHistory= new SearchHistory($searchHistory);
        // Another hack, this is the case when a user chooses to view all placement records of a student. 
        // Sometimes the advanced search's searchHistory object gets passed to here, so it messes up this
        // case where we want to see all placements for the student, some search parameters are left over
        // from the advanced search .
        $grouping=$searchHistory->grouping;
        $search_by_term=$searchHistory->search_by_term;
        $search_by_student=$searchHistory->search_by_student;
        $student_name_select=$searchHistory->student_name_select;
        $student_num=$searchHistory->student_num;

        $termsused=$searchHistory->termsused;
        $term=$searchHistory->term;
        $year=$searchHistory->year;

        $arrName = explode (",",$student_name_select);
        $last_name = trim($arrName[0]);
        $first_name = trim($arrName[1]);
        $department_id = $grouping;
    } 

    // searchHistory object hasn't been declared yet, so lets create that object. 
    else 
    {
        for ($i=1;$i<=sizeof($termsused);$i++)
        {
            if ($termsused[$i] != $i)
            {
                $term[$i]=NULL;
                $year[$i]=NULL;
            }
        }

        $searchHistory = new SearchHistory('');
        
        if ($student_num != NULL)
        {
            unset($student_number_select);
            unset($student_name_select);
        }
        if($student_number_select != NULL && $student_num == NULL)
        {
            $student_num = $student_number_select;
            unset($student_number_select);
            unset($student_name_select);
            $all_groups = true;
            $grouping = 0;
        }

        if ($student_name_select != NULL && $student_num == NULL)
        {
            $arrName = explode (",",$student_name_select);
            $last_name = trim($arrName[0]);
            $first_name = trim($arrName[1]);
            $all_groups = true;
            $grouping = 0;

        }

        $searchHistory->setVariable(
                NULL,NULL,NULL,$student_name_select,$student_num_search_type,$student_num,$first_name,$last_name,$gender,$grouping,$discipline_id,$academic_year
                ,$advisor,$coop_advisor,$citizen,$email_search_type,$email,$work_email_search_type,$work_email,$job_code,$work_term_number,$work_term_length,$salary
                ,$salary_period,$wt_status,$work_term_city,$work_term_region,$work_term_province,$work_term_country,$company_city,$company_region,$company_province
                ,$company_country,$supervisor_name,$site_visit_date_supervisor_equality,$site_visit_date_supervisor,$site_visit_by_supervisor
                ,$site_visit_type_id_supervisor,$site_visit_date_equality,$site_visit_date,$site_visit_by,$site_visit_type_id,$report_code,$report_subject
                ,$report_marker,$termsused,$term,$year,$flags,$nflags,$column,$order
                );
    }


    if ($userlevel == OFFICE)
    {
        if ($department_id && $department_id != 0 && (!$all_groups))
        {
            if ($department_id != "0" && !in_array($department_id, $departments_in_group))
            {
                $error = ("You do not have access to this department");
                $department_id = $auth->department;
                $page = "";
                unset( $PDF );
            }

            else
            {
                $department_in_str = "'".$department_id."'";
            }
        }
        else
        {
            $department_id = $auth->department;

            if ($department_id != "0" && !in_array($department_id, $departments_in_group))
            {
                $error = ("You do not have access to this department");
                $department_id = $auth->department;
                $page = "";
                unset( $PDF );
            }
            else
            {
                $departments_in_group = department_groups($department_id);
            }

            $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
        }
    }

    else
    {
        $error = ("You do not have access to this page.");
    }

    if ($changetodepartment=="Change" || $order<>NULL)
    {
       // echo "<form action='".$PHP_SELF."&amp;select=results' method='post'>";
        
       $order_href = $PHP_SELF . "&amp;select=results&amp;department_id=" . urlencode($department_id);

        global $departments_in_group;
        global $department_id;
        global $department_in_str;
        global $page;
        global $auth;

        if ($order == "")
        {
            $order="A";
        }

        echo("<table cellspacing='0' align='center' class='row1' width='90%' border='0' cellpadding='10'>");
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


        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("<form method='post' action='".$PHP_SELF."&amp;select=history_choose&amp;searchmode=simple'>");
        echo("<input type='hidden' name='search_by_student' value='true' />");
        echo("<b class='black'>");
        echo("Please enter a student number, name, or choose a student from the list <br />");
        echo("below, and select the term(s) for which you wish to view history records.<br /><br />");
        echo("  If entering a student's name, it must be done in the format <i>Last Name, First Name.</i><br />");
        echo("  You may enter only the beginning of either the first or last name, but they must be separated by a comma.<br />");
        echo("</b>");
        echo("</form>");
        echo("</td>");
        echo("</tr>\n");
        ?>

            <tr>
            <td width="30%" align="right">Student Number:</td>
            <td align="left"><input type="text" size="10" name="student_number_select" /></td>
            </tr>


            <tr>
            <td align="right">Student Name:</td>
            <td align="left"><input type="text" size="30" name="student_name_select" /></td>
            </tr>

            <tr>
            <td colspan="2" align="center">
            <?php
            $letter_sql="   SELECT distinct SUBSTRING(s.last_name, 1, 1) AS order_letter
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
            WHERE s.last_name LIKE '" . $order . "%' AND sfj.record_id=smd.record_id AND sfj.student_flags_id=".CURRENT_FLAG.
            " AND (smd.department_id IN (".$department_in_str."))
            ORDER BY s.last_name";

        $result = $GLOBALS['dbh']->Execute($sql);

        $letter_results=$GLOBALS['dbh']->Execute($letter_sql);
        while ($letter_row=$letter_results->FetchRow()){
            if (strtolower($lastLetter)<>strtolower($letter_row['order_letter'])){
                ?>
                    <a class = "blue" href="<?php echo ($order_href) ?>&amp;order=<?php echo $letter_row['order_letter']?>">
                    <?php echo strtoupper($letter_row['order_letter'])?></a> |
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
                    &nbsp;&nbsp;<input type="submit" name="changetodepartment" value="Change" /></td>
                    </tr>
                    <tr><td colspan='2' align='center'>
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
                        $selected='sselected=\'selected\'';
                    }
                    echo("<option value='".$term_row['term_id']."' ". $selected.">".$term_row['term_name']."</option>");
                    $selected=NULL;

                }
                $results->Move(0);
                echo("</select>");

                echo("<select name='year[".$i."]'>");

                $this_year=date("Y");
                for ($year=$start_year;$year<$this_year+2;$year++)
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

        echo("<tr>");
            echo("<td class='row1' colspan='2' align='center'><center><input type='checkbox' name='all' value='true' />All Terms</center></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td class='row1' colspan='2' align='center'><hr />");
            echo("<input type='hidden' name='gender' value='A' />");
            echo("<input type='hidden' name='search_by_term' value='true' />");
            echo("<hr /><input type='submit' name='btnSubmit' value='View Placements' /><input type='reset' value='Reset Form'></td>");
         echo("</tr>");
        echo("</table>");
        echo("</form>");
    }

    if ($student_name_select == NULL && $student_number_select == NULL && $student_num == NULL 
        && $termsused == NULL && $neworder == NULL && $all)
    {
        notify("You must enter some information.");
    }
    else
    {

        $terms_sql = NULL;

        if ($termsused <> NULL)
        {
            $terms_sql = " and (";
            foreach($termsused as $x)
            {
                $terms_sql = $terms_sql."h.year=".$year[$x]." and h.term_id=".$term[$x]. " or ";
            }

            $terms_sql = substr($terms_sql,0,strlen($terms_sql)-4).")";
        }

        $sql = ("
            SELECT s.last_name, s.first_name, s.student_number, h.company_name, h.history_id, h.job_title, h.year, t.term_name, sd.department_id
            , IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number
            FROM student s
            INNER JOIN student_department sd
            ON s.student_number = sd.student_number
            INNER JOIN history h
            ON (sd.student_number = h.student_number) AND (sd.department_id = h.department_id)
            INNER JOIN term t
            ON h.term_id = t.term_id
            LEFT JOIN history_container hc
            ON h.history_id = hc.history_id
            WHERE sd.department_id IN (".$department_in_str.") ".$terms_sql."
            ");
        
        //find out what we should search by:
        if ($student_number_select)    //if a student number was entered in the appropriate text field
        {
            $sql = $sql." AND s.student_number='".$student_number_select."'"; 
        }
        elseif ($student_name_select)  //if a student's name was entered in the appropriate text field
        {
            $anArray=explode(" ",$student_name_select);
            $sql = $sql." AND s.first_name LIKE '".$first_name."%' AND s.last_name LIKE '".$last_name."%'";
        } 
        elseif ($student_num)          //if a student number was selected from the list
        {
            $sql = $sql." AND s.student_number='".$student_num."'";
        }
        
        //find out how we should order the results:
        if ($neworder)   
        {
            $sql = $sql." ORDER BY ".$neworder;
        }
        else
        {
            $sql = $sql." ORDER BY s.last_name,s.first_name, work_term_number";
        }
        
        //if there is something to search for:
        if ($student_num || $student_name_select || $student_number_select || $neworder)
        {
            $results=$GLOBALS['dbh']->Execute($sql);  //then search the database
            $execute="true";          // ?
        }
        else
        {
            $results = $GLOBALS['dbh']->Execute($sql);
        }
       //!!!!!!!!! 
        //Set default number of results displayed per page
        if (($search_by_term=="true" || $search_by_student=="true") && $changetodepartment<>"Change" || $neworder != NULL)
        {
            if ($per_page_max==NULL)   //if no max results entered
            {
                $per_page_max=50;      //default to 50 results per page
            }
            
            if ($per_page_max<5)       //if max results less then 5
            {
                $per_page_max=5;       //default to 5 results per page
            }
            $lastid = NULL;
            $count=0;
            
            //Search through database results to find how many records (array rows) were found
            while ($row = $results->FetchRow())  //while there are rows to fetch
            {
                if ($row['history_id']<>$lastid)    
                {
                    $count=$count+1;                 //increment number of rows
                }
                $lastid=$row['history_id'];
            }

            if ($count==0)   // No results were returned 
            {
                notify("No records exist.");   //So notify user of this fact
            }

            $pages=ceil($count/$per_page_max); //calculate number of pages needed 
            $searchHistory->search_by_term = 'true';
            $searchHistory->search_by_student = 'true';

            if ($count > $per_page_max)   //if we have too many results to display on one page
            {
                echo("<form method='post' action='".$PHP_SELF."'>");
                echo("<input type='hidden' name='select' value='history_choose' />");
                echo("<input type='hidden' name='searchmode' value='simple' />");
                echo("<input type='hidden' name='btnSubmit' value='View Placements' />");
                echo("<input type='hidden' name='limit' value='true' />");
                echo("<input type='hidden' name='termsused' value='".packObject(($termsused))."' />");
                echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
                echo("<input type='hidden' name='neworder' value='".$neworder."' />");
                echo("<table border='0' cellspacing='0' cellpadding='4' class='row1'>");
                echo("<tr>");
                echo("<td>".$count ." student" . (($count != 1) ? "s" : "") . " on ".$pages." page" . (($pages > 1) ? "s" : "") . ";<br /></td>");
                echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='".$per_page_max."' /> records per page.</td>");
                echo("</tr>");
                echo("<tr>");
                echo("<td align='right'>");
                echo("<select name='start_row'>");

                for ($i = 0, $row_num_start = 0; $i < $pages; $i++, $row_num_start += $per_page_max)
                {       
                    $row_num_end = $row_num_start + $per_page_max - 1;
                    if ($row_num_end >= $count) $row_num_end = $count - 1;
                    $result=$GLOBALS['dbh']->Execute($sql);

                    $result->Move($row_num_start);
                    $job_code_row = $result->FetchRow();
                    $job_code_start = $job_code_row[0];
                    
                    $result->Move($row_num_end);
                    $job_code_row = $result->FetchRow();
                    $job_code_end = $job_code_row[0];

                    echo("<option value='".$row_num_start."'>" . $job_code_start . " to " . $job_code_end . "</option>\n");
                }

                echo("</select>");
                echo("</td>");
                echo("<td><input type='submit' value='View' /></td>");
                echo("</tr>");
                echo("</table>");
                echo("</form>");
            }
            elseif ($count < $per_page_max && $count)  //if we can fit all results on this page and there are results to display
            {
                echo("<h4>".$count." Record".(($count != 1) ? "s" : "").":</h4>");  //put a title: "#" Record(s)
            }

            if ($start_row == '') 
            { 
                $start_row = 0; 
            }

            $first = $start_row + 1;
            $end = $start_row + $per_page_max;

            if ($end > $count) 
            { 
                $end = $count; 
            }

            $exresults = $GLOBALS['dbh']->Execute ($sql);

            $history_list = null;
            while ($exrow = $exresults->FetchRow()) 
            {
                $export_string .= "\n". $exrow["work_term_number"] ."\t". $exrow["first_name"]." ".$exrow["last_name"]."\t". $exrow["job_title"] 
                    ."\t". $exrow["company_name"] ."\t". $exrow["term_name"]."\t". $exrow["year"];
                $history_list .= $exrow["history_id"].",";
            }
            if (substr($history_list,-1)==",")
            {
                $history_list=substr($history_list,0,strlen($history_list)-1);
            }

            $display_sql = $sql . "  LIMIT $start_row, $per_page_max";

            $results = $GLOBALS['dbh']->Execute($display_sql);
            if ($execute=="true" || $search_by_term=="true")
            {
                if ($results->RecordCount()>0)
                {
                   // echo("<form method='post' action='".$PHP_SELF."'>");
                //    echo("<table>");
                  //  echo("<tr>");
                    //echo("<td>");
                    echo("<form method='post' action='".$PHP_SELF."'>");
                    echo("<input type='hidden' name='select' value='history' />");
                    echo("<input type='hidden' name='showquick' value='".$showquick."' />");
                    echo("<input type='hidden' name='fromPage' value='simple search' />");
                    echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
                    echo("<input type='hidden' name='history_list' value='".$history_list."' />");
                    echo("<input type='hidden' name='export_string' value='".urlencode(serialize($export_string))."' />");
                    echo("<input type='hidden' name='sql_or_string' value='string' />");

                    echo("<table border='1' cellspacing='0' cellpadding='0'>");
                    echo("<tr>");
                    echo("<td>");
                    echo("<table border='0' cellpadding='2'>");
                    echo("<tr class='rowgrey'>");
                    echo("<td align='center'>&nbsp;<b class='white'>&nbsp;</b>&nbsp;</td>");

                    // This block of code prints out the headers columns. If you cick on one of these headers, it'll sort all information by
                    // that header. 
                    $columns_array[] = array("value" => "work_term_number, s.last_name, s.first_name",
                            "name" => "WT#");
                    $columns_array[] = array("value" => "s.last_name, s.first_name",
                            "name" => "Student");
                    $columns_array[] = array("value" => "h.job_title, s.last_name, s.first_name",
                            "name" => "Job Title");
                    $columns_array[] = array("value" => "h.company_name, s.last_name, s.first_name",
                            "name" => "Company Name");
                    $columns_array[] = array("value" => "t.term_name, s.last_name, s.first_name",
                            "name" => "Term");
                    $columns_array[] = array("value" => "h.year, s.last_name, s.first_name",
                            "name" => "Year");

                    $count = 0;
                    $header_export_string = "";
                    $form_string = null;
                    for ($i=0;$i<sizeof($columns_array);$i++)
                    {
                        $header = $columns_array[$i]["name"];
                        if ($header != "")
                        {   
                            echo("<td align='center' nowrap='nowrap'>");
                            // nested forms aren't allowed, so we print these forms at the bottom of the page. 
                            ob_start();
                            echo("<form name='form".$count."' action='".$PHP_SELF."&amp;select=history_choose&amp;searchmode=simple' method='post'>");
                            echo("<input type='hidden' name='select' value='history_choose' />");
                            echo("<input type='hidden' name='searchmode' value='simple' />");
                            echo("<input type='hidden' name='btnSubmit' value='View Placements' />");
                            echo("<input type='hidden' name='limit' value='true' />");
                            echo("<input type='hidden' name='termsused' value='".packObject(($termsused))."' />");
                            echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
                            echo("<input type='hidden' name='neworder' value='".$columns_array[$i]['value']."' />");
                            echo("<input type='hidden' name='student_num' value='".$student_num."' />");
                            echo("<input type='hidden' name='student_number_select' value='".$student_number_select."' />");
                            echo("<input type='hidden' name='student_name_select' value='".$student_name_select."' />");
                            echo("</form>");
                            $form_string .= ob_get_contents();
                            ob_end_clean();
                            echo("<a href='javascript:document.form".$count.".submit()'>&nbsp;<b class='white'>$header</b>&nbsp;</a>");
                            echo("</td>");
                            $header_export_string .= $header."\t";
                        }
                        $count++;
                    }

                    echo("</tr>");
                    $export_string = $header_export_string.$export_string;
                    $count=0;
                    $i = 0;
                    while ($row = $results->FetchRow())
                    {
                        $current_wt_num = (int) $row['work_term_number'];

                        if (($student_num || $student_number_select || $student_name_select) && useContainers($row['student_number'],$row['department_id']))
                        {
                            if ($i == 0)
                            {
                                $last_wt_num = (int) $row['work_term_number'];
                                $i++;
                            }

                            if ($current_wt_num != $last_wt_num)
                            {
                                $last_wt_num = $current_wt_num;
                                $count++;
                            }
                        }
                        else
                        {
                            $count++;
                        }
                        //if ($row['history_id']<>$lastHistoryID)
                        {
                            //$history_list=$history_list.$row['history_id'].",";
                            if ($count%2==0)
                            {
                                $class='row0d';
                            }
                            else
                            {
                                $class='row1d';
                            }
                            echo("<tr>");
                            echo("<td align='center' valign='middle' class='".$class."' nowrap='nowrap'>");
                            echo("&nbsp;");
                            echo("<input type='radio' name='history_id' value='".$row['history_id']."' onclick='submit()' />");
                            echo("&nbsp;");
                          echo("</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;".$row['work_term_number']."&nbsp;</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;<a href='".$PHP_SELF."&amp;select=history&amp;history_id=".$row['history_id']."'>".$row['first_name']." ".$row['last_name']."</a>&nbsp;</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;".$row['job_title']."&nbsp;</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;".$row['company_name']."&nbsp;</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;".$row['term_name']."&nbsp;</td>");
                            echo("<td align='center' class='".$class."'>&nbsp;".$row['year']."&nbsp;</td>");

                            echo("</tr>");
                            //$count=$count+1;
                            //$lastHistoryID=$row['history_id'];
                        }
                    }

                    /*if (substr($history_list,-1)==",")
                      {
                      $history_list=substr($history_list,0,strlen($history_list)-1);
                      }
                     */
                    echo("</table>");
                   
                    echo("</td>");
                    echo("</tr>");
                    
                   echo("</table>");
                   echo("</form>");
                   
                   echo("<table>"); 
                 //  echo("<tr>");
                 //  echo("<td>");
                 //  echo("<form method='post' action='".$PHP_SELF."'>");
                //   echo("<input type='hidden' name='select' value='history' />");
                //   echo("<form>");
                //   echo("</td>");
                 //  echo("</tr>");

                   echo("<tr>");
                   echo("<td>");
                   echo("<form method='post' action='".$PHP_SELF."'>");
                    echo("<input type='submit' name='btnSubmit' value='Submit' />");
                    echo("<input type='hidden' name='showquick' value='$showquick' />");
                    echo("<input type='hidden' name='fromPage' value='simple search' />");
                    echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
                    echo("<input type='hidden' name='history_list' value='".$history_list."' />");
                    echo("<input type='submit' name='export' value=' Export to File ' />");
                    echo("<input type='hidden' name='export_string' value='" . packObject($export_string)."' />");
                echo("</form>");
                 echo("</td>");
                 echo("</tr>");
                    
                 // if you want to do anything else with the results, just use the following hook
                 $arr_params = array();
                 $arr_params['history_id_list'] = $history_list;
                 $arr_params['PHP_SELF'] = $PHP_SELF;

                 $arr_buttons  = get_hooks('mamook.history_search_results.button', $arr_params);
                 if (is_array($arr_buttons) && sizeof($arr_buttons)) {
                     echo("<tr>");
                     echo("<td>");
                     echo("&nbsp;");
                     echo("</td>");
                     echo("</tr>");
                     echo("<tr>");
                     echo("<td>");
                     for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                         echo($arr_buttons[$i]['button']).'&nbsp;';
                     }
                     echo("</td>");
                     echo("</tr>");
                 }

                echo("</table>");
                // display the forms of the buttons from the plugins need
                if (is_array($arr_buttons) && sizeof($arr_buttons)) {
                    for ($i = 0; $i < sizeof($arr_buttons); $i++) {
                        echo($arr_buttons[$i]['form']);
                    }
                }

                echo($form_string);

                } // end if
            } //end if
        } // end if
    }
