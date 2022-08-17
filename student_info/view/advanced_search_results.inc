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
 | Filename: advanced_search_results.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This file is called to display the results of an advanced       | 
 | search. It paginates the results as well.                                    |
 +------------------------------------------------------------------------------+

*/
    if ($userlevel == OFFICE || $userlevel == FACULTY)
    {
        if ($department_id == "")
        {
            $department_id = $auth->department;
        }

        if ($department_id != "0" && !in_array($department_id, $departments_in_group))
        {
            $error = ("You do not have access to this department");
            $department_id = $auth->department;
            $page = "";
            unset( $PDF );
        }
        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
        $department_in_str="(".$department_in_str.")";
    }

    if ($searchStudents->department != "0")
    {
        $department_in_str = "('".$searchStudents->department."')";
    }
    
    $sql = ("
        SELECT DISTINCT sd.record_id, s.email,");

    $group_flag = 0;
    if (sizeof($searchStudents->column) > 0 && is_array($searchStudents->column))
    {
        foreach($searchStudents->column as $value)
        {
            if ($value && $value != 's.email')
            {
                $sql .= $value.",";
            }
            if ($value == 'MAX(h.work_term_number) AS num_work_terms')
            {
                $group_flag = 1;
            }
        }
    }

    if (sizeof($searchStudents->order) > 0 && is_array($searchStudents->order) && !$group_flag)
    {
        foreach ($searchStudents->order as $value)
        {
            if ($value == 'num_work_terms')
            {
                $sql .= "MAX(h.work_term_number) AS num_work_terms,";
                $group_flag = 1;
            }
        }
    }

    if ($group_flag || (strlen($searchStudents->work_terms_completed) && $searchStudents->work_terms_completed_equality))
    {
        $sql .= "MAX(h.work_term_number) AS num_work_terms,";
        $group_flag = 1;
    }

    trim($sql);
    if (substr($sql,-1)==",")
    {
        $sql = substr($sql,0,strlen($sql)-1);
    }

    $sql .= ("
        FROM student AS s
        INNER JOIN student_department sd
        ON s.student_number = sd.student_number
        ");
    
    if ($group_flag || (strlen($searchStudents->work_terms_completed) && $searchStudents->work_terms_completed_equality))
    {
        $sql .= ("
            LEFT JOIN history h
            ON (sd.department_id = h.department_id AND sd.student_number = h.student_number)
            ");
        $group_flag = 1;
    }
    
    $d_flag = 0;
    $disc_flag = 0;
    $curcl_flag = 0;
    $curpl_flag = 0;
    $currl_flag = 0;
    $permcl_flag = 0;
    $permpl_flag = 0;
    $permrl_flag = 0;
    $tr_flag = 0;
    $coopadvc_flag = 0; // Contact (Co-op Advisor)
    
    foreach ($searchStudents->column as $select_column)
    {
        // Everything in $searchStudents->column is of the form (table_alias).(table_column) or (table_alias).(table_column) AS (table_alias). So to extract the
        // table alias, we use the follow regular expression which grabs everything before the '.'. With this table alias, we can determine which tables we need
        // to join with once. If we enter one of the switch cases, a flag is set. This flag informs us that we have already joined with this table, we don't
        // need to join again.

        $prefix = preg_replace("/(\w+)\..*/","\\1",$select_column);
        switch ($prefix)
        {
            case "d":
                if (!$d_flag)
                {
                    $sql .= ("
                        LEFT JOIN department d
                        ON sd.department_id = d.department_id
                        ");
                    $d_flag = 1;
                }
            break;

            case "tr":
                if (!$tr_flag)
                {
                    $sql .= ("
                            LEFT JOIN transcript tr
                            ON sd.student_number = tr.student_number
                            ");
                    $tr_flag = 1;
                }
            break;

            case "disc":
                if (!$disc_flag)
                {
                    $sql .= ("
                        LEFT JOIN discipline disc
                        ON sd.discipline_id = disc.discipline_id
                        ");
                    $disc_flag = 1;
                }
                break;
            case "curcl":
                if (!$curcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list curcl
                        ON s.country_current = curcl.country_id
                        ");
                    $curcl_flag = 1;
                }
                break;

            case "curpl":
                if (!$curpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list curpl
                        ON s.province_current = curpl.provstate_id
                        ");
                    $curpl_flag = 1;
                }
                break;

            case "currl":
                if (!$currl_flag)
                {
                    $sql .= ("
                            LEFT JOIN region_list currl
                            ON s.current_region_id = currl.region_id
                            ");
                    $currl_flag = 1;
                }
                break;
                
                case "permcl":
                if (!$permcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list permcl
                        ON s.country_permanent = permcl.country_id
                        ");
                    $permcl_flag = 1;
                }
                break;
            case "permpl":
                if (!$permpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list permpl
                        ON s.province_permanent = permpl.provstate_id
                        ");
                    $permpl_flag = 1;
                }
                break;
            case "permrl":
                if (!$permrl_flag)
                {
                    $sql .= ("
                            LEFT JOIN region_list permrl
                            ON s.permanent_region_id = permrl.region_id
                            ");
                    $permrl_flag = 1;
                }
                break;
            case "coopadvc":
                if (!$coopadvc_flag)
                {
                    $sql .= ("
                            LEFT JOIN contact coopadvc
                            ON coopadvc.contact_id = sd.coop_advisor
                            ");
                    $coopadvc_flag = 1;
                }
                break;
        }
    }
    unset($prefix);
    
    foreach ($searchStudents->order as $order_column)
    {
        // Everything in $searchStudents->order is of the form (table_alias).(table_column) or (table_alias).(table_column) AS (table_alias). So to extract the
        // table alias, we use the follow regular expression which grabs everything before the '.'. With this table alias, we can determine which tables we need
        // to join with once. If we enter one of the switch cases, a flag is set. This flag informs us that we have already joined with this table, we don't
        // need to join again.

        $prefix = preg_replace("/(\w+)\..*/","\\1",$order_column);
        switch ($prefix)
        {
            case "d":
                if (!$d_flag)
                {
                    $sql .= ("
                        LEFT JOIN department d
                        ON sd.department_id = d.department_id
                        ");
                    $d_flag = 1;
                }
                break;
            case "disc":
                if (!$disc_flag)
                {
                    $sql .= ("
                        LEFT JOIN discipline disc
                        ON sd.discipline_id = disc.discipline_id
                        ");
                    $disc_flag = 1;
                }
                break;
            

            case "tr":
                if (!$tr_flag)
                {
                    $sql .= ("
                        LEFT JOIN transcript tr
                        ON sd.student_number = tr.student_number
                        ");
                    $tr_flag = 1;
                }
            break;

            case "curcl":
                if (!$curcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list curcl
                        ON s.country_current = curcl.country_id
                        ");
                    $curcl_flag = 1;
                }
                break;
            case "curpl":
                if (!$curpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list curpl
                        ON s.province_current = curpl.provstate_id
                        ");
                    $curpl_flag = 1;
                }
                break;

            case "currl":
                if (!$currl_flag)
                {
                    $sql .= ("
                            LEFT JOIN region_list currl
                            ON s.current_region_id = currl.region_id
                            ");
                    $currl_flag = 1;
                }
                break;
                
            case "permcl":
                if (!$permcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list permcl
                        ON s.country_permanent = permcl.country_id
                        ");
                    $permcl_flag = 1;
                }
                break;
            case "permpl":
                if (!$permpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list permpl
                        ON s.province_permanent = permpl.provstate_id
                        ");
                    $permpl_flag = 1;
                }
                break;

            case "permrl":
                if (!$permrl_flag)
                {
                    $sql .= ("
                            LEFT JOIN region_list permrl
                            ON s.permanent_region_id = permrl.region_id
                            ");
                    $permrl_flag = 1;
                }
                break;

            case "coopadvc":
                if (!$coopadvc_flag)
                {
                    $sql .= ("
                            LEFT JOIN contact coopadvc
                            ON coopadvc.contact_id = sd.coop_advisor
                            ");
                    $coopadvc_flag = 1;
                }
                break;
        }
    }

    // For flags we need to do multiple self joins for each flag the user wants to search for.
    if (is_array($searchStudents->flags) && sizeof($searchStudents->flags) > 0)
    {
        $i = 0;
        foreach ($searchStudents->flags as $value)
        {
            // An example of what this looks like after the loop is complete. (Assume user searches for 3 flags)

            // INNER JOIN student_flags_join sfj1
            // ON sd.record_id = sfj1.record_id         // we first link up with student department table
            // INNER JOIN student_flags_join sfj2
            // ON sfj1.record_id = sfj2.record_id       // then we link the first student_flags_join table with the second, etc.
            // INNER JOIN student_flags_join sfj3
            // ON sfj2.record_id = sfj3.record_id
            $next = $i+1;
            if ($i == 0)
            {

                $sql .= ("
                    INNER JOIN student_flags_join sfj".$next."
                    ON sd.record_id = sfj".$next.".record_id
                    ");
            }

            else
            {
                $sql .= ("
                    INNER JOIN student_flags_join sfj".$next."
                    ON sfj".$i.".record_id = sfj".$next.".record_id
                    ");
            }
            $i++;
        }
    }

    // Begin multiple self-join on eligible placed table. The if statement asserts that all the arrays exist and are of equal sizes. 
    if ((is_array($searchStudents->eligible_placed_used) && sizeof($searchStudents->eligible_placed_used))
        && (is_array($searchStudents->eligible_placed_flags) && sizeof($searchStudents->eligible_placed_flags))
        && (is_array($searchStudents->eligible_placed_term) && sizeof($searchStudents->eligible_placed_term))
        && (is_array($searchStudents->eligible_placed_year) && sizeof($searchStudents->eligible_placed_year))
       )
    {
        $i = 0;
        $last_index;
        foreach($searchStudents->eligible_placed_used as $current_index => $eligible_placed_value)
        {
            if ($i == 0)
            {
                $last_index = $current_index;
            }
            // An example of what this looks like after the loop is complete. (Assume user searches for 3 flags)

            // INNER JOIN eligible_placed ep1
            // ON sd.record_id = ep1.record_id         // we first link up with student_department table
            // INNER JOIN eligible_placed ep2
            // ON ep1.record_id = ep2.record_id        // then we link the first eligible_placed table with the second, etc.
            // INNER JOIN eligible_placed ep3
            // ON ep2.record_id = ep3.record_id
            if ($i == 0)
            {
                $sql .= ("
                    INNER JOIN eligible_placed ep".$current_index."
                    ON sd.record_id = ep".$current_index.".record_id
                    ");
            }

            else
            {
                $sql .= ("
                    INNER JOIN eligible_placed ep".$current_index."
                    ON ep".$last_index.".record_id = ep".$current_index.".record_id
                    ");
            }
            $last_index = $current_index;
            $i++;
        }
    }

    if ($searchStudents->semester_id && $searchStudents->semester_term && $searchStudents->semester_year)
    {
        $sql .= ("
            INNER JOIN semesters_table st
            ON sd.record_id = st.record_id
            ");
    }

    $sql .= ("
        WHERE sd.department_id IN ".$department_in_str."
        ");

    if (strlen(trim($searchStudents->student_num)))
    {
        if ($searchStudents->student_num_search_type == "starts_with")
        {
            $sql .= ("
                AND sd.student_number LIKE '".addslashes(stripslashes($searchStudents->student_num))."%'
                ");
        }

        elseif ($searchStudents->student_num_search_type == "ends_with")
        {
            $sql .= ("
                AND sd.student_number LIKE '%".addslashes(stripslashes($searchStudents->student_num))."'
                ");
        }

        else
        {
            $sql .= ("
                AND sd.student_number = '".addslashes(stripslashes($searchStudents->student_num))."'
                ");
        }
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------------------//
    if ($searchStudents->first_name)
    {
        $sql .= (" 
            AND s.first_name LIKE '".addslashes(stripslashes($searchStudents->first_name))."%' 
            ");
    }
    
    if ($searchStudents->last_name)
    {
        $sql .= (" 
            AND s.last_name LIKE '".addslashes(stripslashes($searchStudents->last_name))."%'
            ");
    }
    
    if ($searchStudents->gender)
    {
        $sql .= (" 
            AND s.gender = '".addslashes(stripslashes($searchStudents->gender))."'
            ");
    }
    
    if ($searchStudents->discipline_id)
    {

        $j = 0;
        $flag = 0;
        for($i = 1; $i < (sizeof($searchStudents->discipline_id)); $i++)
        {
            if(($searchStudents->discipline_id[$i]) != "" && ($searchStudents->discipline_id[$i] != NULL))
            {
                $flag = 1;
                $temp_array[$j] = $searchStudents->discipline_id[$i];
                $j++;
            }

        }
        if(($searchStudents->discipline_id[$i]) != "" && ($searchStudents->discipline_id[$i] != NULL))
        {
            $flag =1;
            $temp_array[$j] = $searchStudents->discipline_id[$i];
            $j++;
        }

        if($flag == 1)
        {
            $sql .= (" AND ( ");
            for($i = 0; $i < (sizeof($temp_array))-1; $i++)
            {
                $sql .= "sd.discipline_id = '".$temp_array[$i]."' OR ";
            }
            $sql .= "sd.discipline_id = '".$temp_array[$i]."'";

            $sql .= (" )");

        }
    }
    
    if (strlen($searchStudents->academic_year) && $searchStudents->academic_year_equality && ($searchStudents->academic_year_equality == "<" 
        || $searchStudents->academic_year_equality == "=" || $searchStudents->academic_year_equality == ">"))
    {
        $sql .= ("
            AND sd.academic_year ".$searchStudents->academic_year_equality." '".addslashes(stripslashes($searchStudents->academic_year))."' 
            ");
    }

    if ($searchStudents->grad_year && $searchStudents->grad_year_equality && ($searchStudents->grad_year_equality == "<"
        || $searchStudents->grad_year_equality == "=" || $searchStudents->grad_year_equality == ">"))
    {
        $sql .= ("
            AND sd.grad_year ".$searchStudents->grad_year_equality." '".addslashes(stripslashes($searchStudents->grad_year))."' 
            ");
    }
    
    if (strlen(trim($searchStudents->primary_email)))
    {
        if ($searchStudents->primary_email_search_type == "starts_with")
        {
            $sql .= ("
                AND s.email LIKE '".addslashes(stripslashes($searchStudents->primary_email))."%' 
                ");
        }

        elseif ($searchStudents->primary_email_search_type == "contains")
        {
            $sql .= ("
                AND s.email LIKE '%".addslashes(stripslashes($searchStudents->primary_email))."%' 
                ");
        }

        else
        {
            $sql .= ("
                AND s.email = '".addslashes(stripslashes($searchStudents->primary_email))."' 
                ");
        }
    }

    if (strlen(trim($searchStudents->secondary_email)))
    {
        if ($searchStudents->secondary_email_search_type == "starts_with")
        {
            $sql .= ("
                AND s.email2 LIKE '".addslashes(stripslashes($searchStudents->secondary_email))."%' 
                ");
        }

        elseif ($searchStudents->secondary_email_search_type == "contains")
        {
            $sql .= ("
                AND s.email2 LIKE '%".addslashes(stripslashes($searchStudents->secondary_email))."%' 
                ");
        }

        else
        {
            $sql .= ("
                AND s.email2 = '".addslashes(stripslashes($searchStudents->secondary_email))."' 
                ");
        }
    }

    if ($searchStudents->advisor)
    {
        $sql .= ("
            AND sd.advisor = '".addslashes(stripslashes($searchStudents->advisor))."' 
            ");
    }
    
    if ($searchStudents->coop_advisor)
    {
        $sql .= ("
            AND sd.coop_advisor = '".addslashes(stripslashes($searchStudents->coop_advisor))."'
            ");
    }
    
    if ($searchStudents->citizen)
    {
        $sql .= ("
            AND s.citizen LIKE '".addslashes(stripslashes($searchStudents->citizen))."%' 
            ");
    }

    if ($searchStudents->current_city)
    {
        $sql .= (" 
            AND s.city_current LIKE '".addslashes(stripslashes($searchStudents->current_city))."%'
            ");
    }

    if ($searchStudents->current_province)
    {
        $sql .= (" 
            AND s.province_current = '".addslashes(stripslashes($searchStudents->current_province))."'
            ");
    }

    if ($searchStudents->current_region)
    {
        $sql .= ("
            AND s.current_region_id = '".addslashes(stripslashes($searchStudents->current_region))."'
            ");
    }

    if ($searchStudents->current_country)
    {
        $sql .= (" 
            AND s.country_current = '".addslashes(stripslashes($searchStudents->current_country))."'
            ");
    }

    if ($searchStudents->permanent_city)
    {
        $sql .= (" 
            AND s.city_permanent LIKE '".addslashes(stripslashes($searchStudents->permanent_city))."%'
            ");
    }

    if ($searchStudents->permanent_province)
    {
        $sql .= (" 
            AND s.province_permanent = '".addslashes(stripslashes($searchStudents->permanent_province))."'
            ");
    }

    if ($searchStudents->permanent_region)
    {
        $sql .= ("
            AND s.permanent_region_id = '".addslashes(stripslashes($searchStudents->permanent_region))."'
            ");
    }

    if ($searchStudents->permanent_country)
    {
        $sql .= (" 
            AND s.country_permanent = '".addslashes(stripslashes($searchStudents->permanent_country))."'
            ");
    }

    if ($searchStudents->high_school)
    {
        $sql .= ("
            AND s.high_school LIKE '%".addslashes(stripslashes($searchStudents->high_school))."%'
            ");
    }

    if (strlen($searchStudents->grad_gpa) && $searchStudents->grad_gpa_equality && ($searchStudents->grad_gpa_equality == "<"
        || $searchStudents->grad_gpa_equality == "=" || $searchStudents->grad_gpa_equality == ">"))
    {
        $sql .= ("
            AND sd.grad_gpa ".$searchStudents->grad_gpa_equality." '".$searchStudents->grad_gpa."'
            ");
    }

    if ($searchStudents->admit && $searchStudents->admit_equality && ($searchStudents->admit_equality == "<"
        || $searchStudents->admit_equality == "=" || $searchStudents->admit_equality == ">"))
    {
        $sql .= ("
            AND sd.admit ".$searchStudents->admit_equality." '".addslashes(stripslashes($searchStudents->admit))."'
            ");
    }
    
    if ($searchStudents->birth && $searchStudents->birth_equality && ($searchStudents->birth_equality == "<"
        || $searchStudents->birth_equality == "=" || $searchStudents->birth_equality == ">"))
    {
        $sql .= ("
            AND s.birth ".$searchStudents->birth_equality." '".addslashes(stripslashes($searchStudents->birth))."'
            ");
    }
    
    if(($searchStudents->convocation_month || $searchStudents->convocation_year) && $searchStudents->convocation_equality && 
        ($searchStudents->convocation_equality == "<" || $searchStudents->convocation_equality == "=" || $searchStudents->convocation_equality == ">"))
    {
        $temp_month = addslashes(stripslashes($searchStudents->convocation_month));  //create a variable with a shorter name
        $temp_year = addslashes(stripslashes($searchStudents->convocation_year));    
        $temp_equality = $searchStudents->convocation_equality;                      
        
        if(($temp_month && $temp_year) && (($temp_equality == "<") || ($temp_equality == ">")) )  //if we have a month and a year to search for 
        {                                                                                         //  and we want to find greater/less than that date
            $sql .= ("AND ((sd.convocation_year $temp_equality '$temp_year') 
                      OR (sd.convocation_year = '$temp_year'   
                      AND sd.convocation_month $temp_equality '$temp_month'))");
        }
        else                                                                                      //otherwise set month/year search individually
        {
            if ($temp_month && $temp_equality)                                                    //if a month and equality has been selected for search
            {
                $sql .= ("AND sd.convocation_month $temp_equality '$temp_month' ");               //format ex.: "AND sd.convocation_month > '11'"
            }

            if ($temp_year && $temp_equality)                                                     //if a year and equality has been selected for search
            {
                $sql .= ("AND sd.convocation_year $temp_equality '$temp_year' ");                 //format ex.: "AND sd.convocation_year < '2003'"
            }
        }
    }

    if ($searchStudents->view_job_recent && $searchStudents->view_job_recent_equality && ($searchStudents->view_job_recent_equality == "<"
        || $searchStudents->view_job_recent_equality == "=" || $searchStudents->view_job_recent_equality == ">"))
    {
        $sql .= ("
            AND s.view_job_recent ".$searchStudents->view_job_recent_equality." '".addslashes(stripslashes($searchStudents->view_job_recent))."'
            ");
    }
    
    if ($searchStudents->withdraw && $searchStudents->withdraw_equality && ($searchStudents->withdraw_equality == "<"
        || $searchStudents->withdraw_equality == "=" || $searchStudents->withdraw_equality == ">"))
    {
        $sql .= ("
            AND sd.withdraw ".$searchStudents->withdraw_equality." '".addslashes(stripslashes($searchStudents->withdraw))."'
            ");
    }
    
    if ($searchStudents->start_year && $searchStudents->start_year_equality && ($searchStudents->start_year_equality == "<"
        || $searchStudents->start_year_equality == "=" || $searchStudents->start_year_equality == ">"))
    {
        $sql .= ("
            AND sd.start_year ".$searchStudents->start_year_equality." '".addslashes(stripslashes($searchStudents->start_year))."'
            ");
    }
    
    if (is_array($searchStudents->flags) && sizeof($searchStudents->flags) > 0)
    {
        $i=0;
        foreach($searchStudents->flags as $value)
        {
            $next = $i+1;
            $sql .= ("
                AND sfj".$next.".student_flags_id = '".$value."'
                ");
            $i++;
        }
    }

    if ((is_array($searchStudents->eligible_placed_used) && sizeof($searchStudents->eligible_placed_used))
        && (is_array($searchStudents->eligible_placed_flags) && sizeof($searchStudents->eligible_placed_flags))
        && (is_array($searchStudents->eligible_placed_term) && sizeof($searchStudents->eligible_placed_term))
        && (is_array($searchStudents->eligible_placed_year) && sizeof($searchStudents->eligible_placed_year))
       )
    {
        // $flag determines if we need to start a new "and" statement in SQL. The result of all this will be AND((condition) OR (condition)) 
        $flag = 0;
        // $i is the array index or key.  
        foreach($searchStudents->eligible_placed_used as $i => $eligible_placed_value)
        {
            if ($searchStudents->eligible_placed_used[$i])
            {
                // If we chose to search for Any Term, the value comes back as 1,2,3 (a list of term_ids).
                if (preg_match("/,/",$searchStudents->eligible_placed_term[$i]))
                {
                    // split the term_id list into its individual elements. 
                    $temp_term_array = explode(",",$searchStudents->eligible_placed_term[$i]);
                    for($j=0;$j<sizeof($temp_term_array);$j++)
                    {
                        if ($flag == 0)
                        {
                            $sql .= ("
                                AND ((ep".$i.".term_id IN (".$temp_term_array[$j].") AND ep".$i.".year = '".$searchStudents->eligible_placed_year[$i]."'
                                    AND ep".$i.".student_flags_id = '".$searchStudents->eligible_placed_flags[$i]."'
                                    )
                                ");
                            $flag++;
                        }

                        else
                        {
                            $sql .= ("
                                OR (ep".$i.".term_id IN (".$temp_term_array[$j].") AND ep".$i.".year = '".$searchStudents->eligible_placed_year[$i]."'
                                   AND ep".$i.".student_flags_id = '".$searchStudents->eligible_placed_flags[$i]."'
                                   )
                                ");
                        }
                       
                    }
                }

                else 
                {
                    if ($flag == 0)
                    {
                        $sql .= ("
                            AND ((ep".$i.".term_id IN (".$searchStudents->eligible_placed_term[$i].")
                                AND ep".$i.".year = '".$searchStudents->eligible_placed_year[$i]."'
                                AND ep".$i.".student_flags_id = '".$searchStudents->eligible_placed_flags[$i]."'
                                )
                            ");
                        $flag++;
                    }

                    else
                    {
                        $sql .= ("
                            OR (ep".$i.".term_id IN (".$searchStudents->eligible_placed_term[$i].")
                                AND ep".$i.".year = '".$searchStudents->eligible_placed_year[$i]."'
                                AND ep".$i.".student_flags_id = '".$searchStudents->eligible_placed_flags[$i]."'
                               )
                            ");
                    }
                }
            }
        }

        if ($flag > 0)
        {
            $sql .= (")");
        }
    }

    if ($searchStudents->semester_id && $searchStudents->semester_term && $searchStudents->semester_year)
    {
        $sql .= ("
            AND (st.semesters_id IN (".$searchStudents->semester_id.") AND st.term_id IN (".$searchStudents->semester_term.")
                AND st.year = '".$searchStudents->semester_year."'
                )
            ");
    }

    if ($group_flag)
    {
        $sql .= ("
            GROUP BY s.student_number
            ");
    }
    
    if (strlen($searchStudents->work_terms_completed) && $searchStudents->work_terms_completed_equality)
    {
        $sql .= (" 
            HAVING MAX(h.work_term_number) ".$searchStudents->work_terms_completed_equality." '".addslashes(stripslashes($searchStudents->work_terms_completed))."'
            ");
    }


    $sql .= ("
        ORDER BY 
        ");

    if ($neworder)
    {
        $order_by=$neworder.",";
        if ($neworder == "s.last_name")
        {
            $order_flag = 1;
        }
    }

    elseif (sizeof($searchStudents->order) > 0 && is_array($searchStudents->order))
    {
        for($i = 1;$i<=sizeof($searchStudents->order);$i++)
        {
            if ($searchStudents->order[$i]<>NULL)
            {
                $order_by .= $searchStudents->order[$i].",";
               
                if ($searchStudents->order[$i] == "s.last_name")
                {
                    $order_flag = 1;
                }
            }
        }
    }

    // If we DON'T order by last name, then we tack on s.last_name to the end of the query. Otherwise, we're already searching by last name, so do nothing. 
    if (!$order_flag)
    {
        $order_by .= "s.last_name,";
    }

    $order_by = substr($order_by,0,strlen($order_by)-1);

    $sql .= " ".$order_by;
    $results = $GLOBALS['dbh']->Execute($sql);
    // Pre-count is our initial number of records we get back. 
    $precount = $results->RecordCount();

    
    if ($precount > 0)
    {
        // Get the query's associative array's keys. 

        $arrKeys = array();
        $row = $results->FetchRow();
        $i = 0;
        foreach ($row as $key => $value)
        {
            $arrKeys[$i] = $key;
            $i++;
        }
        $results->Move(0);

        // Copy everything from our last query into the $arrRow array. 
        $arrRow = array();
        $i = 0;
        while ($row = $results->FetchRow())
        {
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
               $arrRow[$i][$arrKeys[$j]] = $row[$arrKeys[$j]]; 
            }
            $i++;
        }
        $results->Move(0);

        // For debugging purposes. 

        /*
        for($i=0;$i<sizeof($arrRow);$i++)
        {
            echo($i.". ");
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
                print $arrRow[$i][$arrKeys[$j]]."\t";
            }
            print "<br />";
        }
        */
       
        // At this point we have all the information we want stored in an array called $arrRow. If someone wants a list of students that does NOT have certain flags
        // set, we enter this next if clause. This block of code checks if any of these unwanted flags have been set for the student record. If so, we remove that 
        // entry from $arrRow by setting arrRow's record_id to NULL. Afterwards, we copy all records with a non-null record_id to a new arrRow array.

        if(is_array($searchStudents->nflags) && sizeof($searchStudents->nflags) > 0)
        {
            //Algorithm #1: This algorithm works better for small sets of numbers and small sets of flags.
            if (sizeof($searchStudents->nflags) == 1 || ($precount <= 600 && sizeof($searchStudents->nflags) <= 10))
            {
                foreach($searchStudents->nflags as $flag_id)
                {
                    for($i=0;$i<sizeof($arrRow);$i++)
                    {
                        if ($arrRow[$i]['record_id'])
                        {
                           $flag_sql = ("
                                SELECT sd.record_id
                                FROM student_department sd
                                INNER JOIN student_flags_join sfj
                                ON sd.record_id = sfj.record_id
                                WHERE sfj.student_flags_id = '".$flag_id."' AND sfj.record_id = '".$arrRow[$i]['record_id']."'
                                ");
                            $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                            if ($flag_result->RecordCount() > 0)
                            {
                                $arrRow[$i]['record_id'] = NULL;
                                $precount--;
                            }
                        }
                    }
                }
            }
            // End Algorithm #1

            // Algorithm #2: This algorithm works better for very large sets of numbers and large sets of flags. 
            else
            {
                $conditions = "";
                $i=0;
                foreach($searchStudents->nflags as $flag_id)
                {
                    if ($i==0)
                    {
                        $conditions .= "sfj.student_flags_id = '".$flag_id."' ";
                        $i++;
                    }

                    else
                    {
                        $conditions .= "OR sfj.student_flags_id = '".$flag_id."' ";
                    }
                }

                $conditions = "(".$conditions.")";

                for($i=0;$i<sizeof($arrRow);$i++)
                {
                    if ($arrRow[$i]['record_id'])
                    {
                        $flag_sql = ("
                            SELECT sd.record_id
                            FROM student_department sd
                            INNER JOIN student_flags_join sfj
                            ON sd.record_id = sfj.record_id
                            WHERE sfj.student_flags_id = '".$flag_id."' AND ".$conditions."
                            ");
                        $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                        if ($flag_result->RecordCount() > 0)
                        {
                            $arrRow[$i]['record_id'] = NULL;
                            $precount--;
                        }
                    }
                }
            }
            // End Algorithm #2
        }
        
        if ((is_array($searchStudents->not_eligible_placed_used) && sizeof($searchStudents->not_eligible_placed_used))
            && (is_array($searchStudents->not_eligible_placed_flags) && sizeof($searchStudents->not_eligible_placed_flags))
            && (is_array($searchStudents->not_eligible_placed_term) && sizeof($searchStudents->not_eligible_placed_term))
            && (is_array($searchStudents->not_eligible_placed_year) && sizeof($searchStudents->not_eligible_placed_year))
            && $precount > 0
           )
        {
            foreach($searchStudents->not_eligible_placed_used as $index => $some_value)
            {
                for($i=0;$i<sizeof($arrRow);$i++)
                {
                    if ($arrRow[$i]['record_id'])
                    {
                        $flag_sql = ("
                            SELECT sd.record_id
                            FROM student_department sd
                            INNER JOIN eligible_placed ep
                            ON sd.record_id = ep.record_id
                            WHERE ep.student_flags_id = '".$searchStudents->not_eligible_placed_flags[$index]."' 
                            AND ep.term_id IN (".$searchStudents->not_eligible_placed_term[$index].")
                            AND ep.year = '".$searchStudents->not_eligible_placed_year[$index]."'
                            AND sd.record_id = '".$arrRow[$i]['record_id']."'
                            ");
                        $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                        if ($flag_result->RecordCount() > 0)
                        {
                            $arrRow[$i]['record_id'] = NULL;
                            $precount--;
                        }
                    }
                }
            }
        }
    }

    // Rebuild array. 
    $tmpArrRow = array();
    $counter = 0;
    for ($i=0;$i<sizeof($arrRow);$i++)
    {
        if ($arrRow[$i]['record_id'] != NULL)
        {
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
                $tmpArrRow[$counter][$arrKeys[$j]] = $arrRow[$i][$arrKeys[$j]];
            }
            $counter++;
        }
    }
    unset($arrRow);
    $arrRow = $tmpArrRow;

    $count = sizeof($arrRow);

    // User came from advanced search. Give them option to go back. 
    if ($btnSearch)
    {
        echo("<table cellspacing='0' cellpadding='0' border='0'>");
        echo("<tr>");
        echo("<td>");
        echo("<form name='new_search_form' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
        echo("<input type='hidden' name='continue' value='Advanced Search' />");
        echo("<input type='submit' name='anonymous' value='New Search' />");
        echo("</form>\n\n");
        echo("&nbsp;");
        echo("</td>");
        echo("<td>");
        echo("<form name='edit_search_form' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
        echo("<input type='hidden' name='continue' value='Advanced Search' />");
        echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
        echo("<input type='submit' name='btnSearch' value='Edit Search Criteria' />");
        echo("</form>\n\n");
        echo("&nbsp;");
        echo("</td>");
        if(!$search_saved && $auth->userlevel == OFFICE)
        {
            echo("<td>");
            echo("<form name='save_search_form' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
            echo("<input type='hidden' name='continue' value='Advanced Search' />"); 
            echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
            echo("<input type='submit' name='btnSearch' value='Save Search' />");
            echo("</form>\n\n"); 
            echo("&nbsp;");
            echo("</td>");
        }
        echo("</tr>\n\n");
        echo("</table>");
    }

    if($count > 0)
    {
        // Build Student List for 'Next' and 'Previous'
        $student_list = NULL;

        for($i=0;$i<sizeof($arrRow);$i++)
        {
            $student_list .= $arrRow[$i]['record_id'].",";
        }

        $student_list = substr($student_list, 0, -1);

        //$student_sql = $sql;
        //$count_students = $GLOBALS['dbh']->Execute($student_sql);

        // This variable tells us how many students were returned from the search query. This is very important throughout the rest of the file.
        //$num_students = $count_students->RecordCount();

        // Paging begins. 
        if ($per_page_max == "") { $per_page_max = 50; }
        if ($per_page_max < 5) { $per_page_max = 5; }
        $pages = ceil($count / $per_page_max);

        if ($count>$per_page_max)
        {
            echo("<form action='".$PHP_SELF."&amp;select=view_student' method='post'>");
            echo("<input type='hidden' name='btnSearch' value='Search' />");
            echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
            echo("<input type='hidden' name='continue' value='Advanced Search' />");

            echo("<table border='0' cellspacing='0' cellpadding='4' class='row1'>");
                echo("<tr>");
                    echo("<td>");
                        echo($count." record" . (($count != 1) ? "s" : "") . " on ".$pages." page" . (($pages > 1) ? "s" : "") . ";<br />"); 
                    echo("</td>");
                    echo("<td>");
                        echo("<input type='text' name='per_page_max' size='4' maxlength='4' value='".$per_page_max."' /> record".(($count !=1) ? "s": "")." per page.");
                    echo("</td>");
                echo("</tr>\n\n");

                echo("<tr>");
                    echo("<td>");
                        echo("<select name='start_row'>");

                        // This if/else clause determines what the paging box will display in its dropdown menu. ie (Abe to Charlie) (Chuckles to Xandu)
                        if ($neworder && $newvalue)
                        {
                            // $newvalue is the $columns_array value attribute. This first if case checks for something that looks like 
                            // s.student_number AS the_num . We want to extract just 'the_num'. This extracted string will be used in arrRow like
                            // $arrRow[$index]['the_num'].
                            if (preg_match("/(.*)\.(.*)AS(.*)/",$newvalue))
                            {
                                $display_column = preg_replace("/(.*)\.(.*)AS(.*)/","\\3",$newvalue);
                                $display_column = trim($display_column);
                            }
                             
                            // This case looks for something like s.student_number and extracts student_number. 
                            elseif (preg_match("/(.*)\.(.*)/",$newvalue))
                            {
                                $display_column = preg_replace("/(.*)\.(.*)/","\\2",$newvalue);
                                $display_column = trim($display_column);
                            }
                        }

                        // By default we list the last names in the dropdown menu. 
                        else
                        {
                            $display_column = "last_name";
                        }

                        for ($i = 0, $row_num_start = 0; $i < $pages; $i++, $row_num_start += $per_page_max)
                        {
                            $row_num_end = $row_num_start + $per_page_max - 1;
                            if ($row_num_end >= $count) $row_num_end = $count - 1;

                            $display_start = $arrRow[$row_num_start][$display_column];
                            $display_end = $arrRow[$row_num_end][$display_column];
                            
                            if ($start_row == $row_num_start)
                            {
                                $selected = "selected='selected'";
                                $current_page = $i+1;
                            }

                            else
                            {
                                $selected = "";
                            }

                            echo("<option value='".$row_num_start."' ".$selected.">" . $display_start . " to " . $display_end . "</option>\n");
                        }

                        echo("</select>");
                    echo("<input type='hidden' name='neworder' value='".$neworder."' />");
                    if ($neworder_display)
                    {
                        echo("<input type='hidden' name='newvalue' value='".$newvalue."' />");
                    }

                    echo("</td>");
                    echo("<td align='right'>");
                        echo("<input type='submit' value='View' />");
                    echo("</td>");
                echo("</tr>\n\n");
            echo("</table>");
            echo("</form>\n\n");
        } 
        
        else 
        {
            echo("<h4>".$count." record".(($count !=1) ? "s": "").":</h4>");
        }
        
        // Variables for paging
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

        echo("<form method='post' action='" . $PHP_SELF . "&amp;select=view_student' name='student_form'>");
        echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
        // The following two hidden fields control paging. 
        echo("<input type='hidden' name='start_row' value='" . $start_row . "' />");
        echo("<input type='hidden' name='per_page_max' value='" . $per_page_max . "' />");
        echo("<input type='hidden' name='continue' value='Chosen Student' />");
        echo("<input type='hidden' name='student_list' value='" . $student_list . "' />");

        echo("<table cellspacing='0' border='1' cellpadding='0'>");
            echo("<tr>");
                echo("<td>");
                    echo("<table cellpadding='2' border='0'>");
                        echo("<tr class='rowgrey'>");
                            echo("<td>&nbsp;</td>");

                            // form_string holds a bunch of form information that will be printed out at the bottom of the page
                            // because XHTML doesn't like nested forms. 
                            $form_string = null;
                            // Display column titles. 
                            for ($i=1;$i<=sizeof($searchStudents->column);$i++)
                            { 
                                $value = $searchStudents->column[$i];

                                if ($value != "") 
                                {
                                    for ($j = 0; $j < sizeof($columns_array); $j++) 
                                    {
                                        if ($columns_array[$j]["value"] == $value) 
                                        {
                                            $header = $columns_array[$j]["name"];
                                            
                                            // Work term takes up too much space, so we compress the header name. 
                                            if ($header == "Work Term Number")
                                            {
                                                $header = "WT#";
                                            }

                                            $matched_index = $j;
                                        }
                                    }

                                    // When a column title is clicked, the results will order by that field. This form handles that. 
                                    ob_start();
                                    echo("<form name='form".$i."' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
                                    echo("<input type='hidden' name='btnSearch' value='Search' />");
                                    echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
                                    echo("<input type='hidden' name='continue' value='Advanced Search' />");
                                    echo("<input type='hidden' name='newvalue' value='".$columns_array[$matched_index]['value']."' />");
                                    echo("<input type='hidden' name='neworder' value='".$columns_array[$matched_index]['order']."' />");
                                    echo("</form>\n\n");
                                    $form_string .= ob_get_contents();
                                    ob_end_clean(); 

                                    echo("<td align='center' nowrap='nowrap'>");
                                    echo("<a href='javascript:document.form".$i.".submit()'>&nbsp;<b class='white'>".$header."</b>&nbsp;</a>");
                                    echo("</td>");

                                    $export_string .= $header."\t";
                                }
                                unset($header);
                            }

                            // Export to file creation. 
                            for($i=0;$i<sizeof($arrRow);$i++)
                            {
                                $export_string .= "\n";

                                $temp_ex = "";
                                $matched_flag = 0;
                                for ($j=1;$j<=sizeof($searchStudents->column);$j++)
                                {
                                    $value = $searchStudents->column[$j];
                                    if ($value<>NULL)
                                    {
                                        // Special formatting cases. 
                                        switch ($value)
                                        {
                                            case "s.street_address_permanent,s.street_address_permanent2,s.street_address_permanent3":
                                                $temp_ex = $arrRow[$i]["street_address_permanent"]." ".$arrRow[$i]["street_address_permanent2"]." ".$arrRow[$i]["street_address_permanent3"];
                                                $matched_flag = 1;
                                                break;
                                            case "s.street_address_current,s.street_address_current2,s.street_address_current3":
                                                $temp_ex = $arrRow[$i]["street_address_current"]." ".$arrRow[$i]["street_address_current2"]." ".$arrRow[$i]["street_address_current3"];
                                                $matched_flag = 1;
                                                break;
                                            case "sd.convocation_year,sd.convocation_month":
                                                if ($arrRow[$i]['convocation_year'] && $arrRow[$i]['convocation_month']) {
                                                    $temp_month = date("F", mktime(0,0,0, $arrRow[$i]["convocation_month"]+1, 0,0));
                                                    $temp_year = $arrRow[$i]["convocation_year"];
                                                    $temp_ex = $temp_month." ".$temp_year;
                                                }
                                                $matched_flag = 1;
                                                break;
                                            case "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name":
                                                $temp_ex = $arrRow[$i]["coopadvc_first_name"]." ".$arrRow[$i]["coopadvc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                        }

                                        // Didn't match a special case, let's see if this value is something like (column) AS (alias). Extract alias. 
                                        if ($matched_flag == 0)
                                        {
                                            if (preg_match("/(.*)AS(.*)/",$value))
                                            {
                                                $match_string_1 = preg_replace("/(.*)AS(.*)/","\\2",$value);
                                                $match_string_1 = trim($match_string_1);
                                            }
                                            if ($match_string_1)
                                            {
                                                $temp_ex = $arrRow[$i][$match_string_1];
                                                $matched_flag = 1;
                                            }
                                        }

                                        // No match yet, lets see if this value is like (table alias).(column). Extract column
                                        if ($matched_flag == 0)
                                        {
                                            if (preg_match("/(.*)\.(.*)/",$value))
                                            {
                                                $match_string_2 = preg_replace("/(.*)\.(.*)/","\\2",$value);
                                                $match_string_2 = trim($match_string_2);
                                            }
                                            if ($match_string_2)
                                            {
                                                $temp_ex = $arrRow[$i][$match_string_2];
                                                $matched_flag = 1;
                                            }

                                        } 

                                        if ($matched_flag == 0)
                                        {
                                            $temp_ex = "";
                                            $matched_flag = 1;
                                        }

                                        $export_string .= $temp_ex."\t";
                                    }
                                    unset($matched_flag);
                                    unset($match_string_1);
                                    unset($match_string_2);
                                    unset($temp_ex);
                                }
                            } // end for


                        echo("</tr>\n\n");

                        $colour = 0;
                        for($i=$first-1;$i<$end;$i++)
                        {
                            $colour++;
                            $class = ($colour%2 == 0) ? "row0d" : "row1d";

                            echo("<tr>");
                                echo("<td align='center' class='" . $class . "' nowrap='nowrap'>");
                                    echo("&nbsp;");
                                    echo("<input type='radio' ".(($colour == 1) ? "checked='checked'" : "")." name='record_id' value='".$arrRow[$i]["record_id"]."' onclick='submit()' />");
                                    echo("&nbsp;");
                                echo("</td>");

                                unset($outstring);
                                unset($matched_flag);
                                unset($match_string_1);
                                unset($match_string_2);
                                $matched_flag = 0;

                                // Display search results. The structure is almost the same as export to file. Refer above for more details.  
                                for($j=1;$j<=sizeof($searchStudents->column);$j++)
                                {
                                    $value = $searchStudents->column[$j];
                                    if ($value<>NULL)
                                    {
                                        switch ($value)
                                        {
                                            case "s.street_address_permanent,s.street_address_permanent2,s.street_address_permanent3":
                                                $outstring = $arrRow[$i]["street_address_permanent"]." ".$arrRow[$i]["street_address_permanent2"]." ".$arrRow[$i]["street_address_permanent3"];
                                                $matched_flag = 1;
                                                break;
                                            case "s.street_address_current,s.street_address_current2,s.street_address_current3":
                                                $outstring = $arrRow[$i]["street_address_current"]." ".$arrRow[$i]["street_address_current2"]." ".$arrRow[$i]["street_address_current3"];
                                                $matched_flag = 1;
                                                break;
                                            case "s.email":
                                                $outstring = "<font size='-1'><a href='mailto:" . $arrRow[$i]['email'] . "'>&nbsp;";
                                                $outstring .= $arrRow[$i]['email'] . "&nbsp;</a></font>";
                                                $matched_flag = 1;
                                                break;
                                            case "s.email2":
                                                $outstring = "<font size='-1'><a href='mailto:" . $arrRow[$i]['email2'] . "'>&nbsp;";
                                                $outstring .= $arrRow[$i]['email2'] . "&nbsp;</a></font>";
                                                $matched_flag = 1;
                                                break;
                                            case "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name":
                                                $outstring = $arrRow[$i]["coopadvc_first_name"]." ".$arrRow[$i]["coopadvc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "sd.convocation_year,sd.convocation_month":
                                                if ($arrRow[$i]['convocation_year'] && $arrRow[$i]['convocation_month']) {
                                                    $temp_month = date("F", mktime(0,0,0, $arrRow[$i]["convocation_month"]+1, 0,0));
                                                    $temp_year = $arrRow[$i]["convocation_year"];
                                                    $outstring = $temp_month." ".$temp_year;
                                                }
                                                $matched_flag = 1;
                                                break;
                                        }

                                        if ($matched_flag == 0)
                                        {
                                            if (preg_match("/(.*)AS(.*)/",$value))
                                            {                                            
                                                $match_string_1 = preg_replace("/(.*)AS(.*)/","\\2",$value);
                                                $match_string_1 = trim($match_string_1);
                                            }
                                            if ($match_string_1)
                                            {
                                            
                                                $outstring = $arrRow[$i][$match_string_1];
                                                $matched_flag = 1;
                                            }
                                        }

                                        if ($matched_flag == 0)
                                        {
                                            if (preg_match("/(.*)\.(.*)/",$value))
                                            {
                                                $match_string_2 = preg_replace("/(.*)\.(.*)/","\\2",$value);
                                                $match_string_2 = trim($match_string_2);
                                            }
                                            if ($match_string_2)
                                            {
                                                $outstring = $arrRow[$i][$match_string_2];
                                                $matched_flag = 1;
                                            }

                                        }

                                        if ($matched_flag == 0)
                                        {
                                            $outstring = "";
                                            $matched_flag = 1;
                                        }

                                        if ($matched_flag == 1)
                                        {
                                            echo("<td align='center' class='" . $class . "' nowrap='nowrap'>&nbsp;" . $outstring . "&nbsp;</td>");
                                        }
                                    }

                                    unset($match_string_1);
                                    unset($match_string_2);
                                    unset($outstring);
                                    unset($matched_flag);
                                }

                            echo("</tr>\n\n");
                        } // end while

                        $column_count=0;
                        foreach ($searchStudents->column as $column_row)
                        {
                            if ($column_row<>NULL)
                            {
                                $column_count=$column_count+1;
                            }
                        }

                        echo("<tr class='" . ($class == "row0d" ? "row1d" : "row0d") . "'>");
                            echo("<td colspan='".($column_count+1)."'>");
                                echo("<input type='submit' value='View Selected Student' /> &nbsp; ");
                                echo("<input type='hidden' name='export' value='' />");
                                echo("<input type='hidden' name='export_string' value='" . packObject($export_string) . "' />");
                            echo("</td>");
                        echo("</tr>\n\n");
                    echo("</table>");
                echo("</td>");
            echo("</tr>\n\n");
        echo("</table>");
        echo("</form>\n\n");
        
        // buttons for operations to be done on the results 
        echo("<table border='0' cellspacing='0' cellpadding='0'>");
        echo("<tr>");
        echo("<td>");
        echo("<input type='button' value=' Export to File ' onclick='javascript:triggerExport(\"student_form\", \"export\", \"Export to File\");' /> ");
        if ($auth->userlevel == OFFICE) {
            echo("<input type='button' value='Set Flags' onclick='javascript:document.set_flags_form.submit()' /> ");
        }
        echo("</td>");
        echo("</tr>\n\n");
        echo("<tr>");
        echo("<td>");
        echo("&nbsp;");
        echo("</td>");
        echo("</tr>\n\n");
        echo("<tr>");
        echo("<td>");
        echo("<input type='button' value='E-mail All Matches' onclick='javascript:document.email_students_form.submit()' /> ");
        echo("</td>");
        echo("</tr>\n\n");

        // plugins
        // if you want to do anything else with the results, just use the following hook
        $arr_params = array();
        $arr_params['record_id_list'] = $student_list;
        $arr_params['PHP_SELF'] = $PHP_SELF;

        $arr_buttons  = get_hooks('mamook.student_search_results.button', $arr_params);
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

        if ($auth->userlevel == OFFICE) {
            echo("<form name='set_flags_form' method='post' action='".$PHP_SELF."&amp;select=set_student_flags'>");
            // Passing record ids to set student flags. This list is being formatted to work in a SQL IN statement.
            echo("<input type='hidden' name='student_id_list' value='".$student_list."' />");
            echo("<input type='hidden' name='form_submitted' value='1' />");
            echo("<input type='hidden' name='advanced_search' value='1' />");
            // student_id_list, form_submitted, and advanced_search is used for set_student_flags.inc
            echo("</form>\n\n");
        }

        for($i=0;$i<$count;$i++)
        {
            // Creates a list of unique student e-mail address. This is to allow a user to e-mail all students found on this search critera. 
            if ($arrRow[$i]["email"])
            {
                if (!is_array($emails_recorded) || !in_array($arrRow[$i]["email"], $emails_recorded))
                {
                    $emails_recorded[] = $arrRow[$i]["email"];
                    $bcc .= (trim($arrRow[$i]["email"]) . ", ");
                }
            }

            // Generate a list of students that we don't have an e-mail for. 
            else
            {
                if (!is_array($students_without_email) || !in_array($arrRow[$i]["student_number"], $students_without_email))
                {
                    $students_without_email[] = ($arrRow[$i]["student_number"]);
                }
            }

        }

        $bcc = trim($bcc);
        if (substr($bcc,-1)==",")
        {
            $bcc = substr($bcc,0,strlen($bcc)-1);
        }

        echo("<form name='email_students_form' method='post' action='".$PHP_SELF."&amp;select=email_students'>");
        echo("<input type='hidden' name='bcc' value='".$bcc."' />");
        echo("<input type='hidden' name='searchStudents' value='" . packObject($searchStudents) . "' />");
        echo("<input type='hidden' name='students_without_email' value='" . packObject($students_without_email) . "' />");
        echo("</form>\n\n");

        echo($form_string);

        ?>
        <script type='text/javascript' language='javascript'>
        <!--javascript

        function triggerExport(form_name, element_name, element_value) {

            form = eval("document." + form_name);
            flag = 0;
            var i; 
            for (i = 0; i < form.elements.length; i++) {
                if (form.elements[i].name == element_name) {
                    form.elements[i].value = element_value;
                    flag = 1;
                    break;
                }
            }

            if (flag == 1) {
                form.submit();
                form.elements[i].value = null;
            }
        }

        //-->
        </script>

        <?php
    }

    else
    {
        echo("<table>");
            echo("<tr>");
                echo("<td>&nbsp;</td>");
            echo("</tr>\n\n");
            echo("<tr>");
                notify("No matches found.");
            echo("</tr>\n\n");
        echo("</table>");
    }
        
?>
