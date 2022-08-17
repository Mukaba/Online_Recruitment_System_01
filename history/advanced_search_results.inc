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
 | Description: This file is called to display the results of the history       |
 | advanced search. It splits the history records by page, so that it doesnt    |
 | overload their browser with too many results. Users can e-mail the students  |
 | that have been matched. They can export the search results to a              |
 | tab-delimited file and download it. They export all the history records to   |
 | a single PDF file and view all their history record information.             |
 +------------------------------------------------------------------------------+

*/

    // Initialize department_in_str

    $department_in_str = "('')";
    if ($userlevel == OFFICE)
    {
        $department_id = $auth->department;
        $group_array = department_groups($department_id);

        if ($department_id == 0)
        {
            $error = 1;
            $page = "";
            unset($PDF);
        }
        
        elseif ($searchHistory->grouping == 0)
        {
            $department_in_str = "('" . implode("','",$group_array) . "')";
        }

        elseif (in_array($searchHistory->grouping,$group_array))
        {
            $department_in_str = "('".$searchHistory->grouping."')";
        }
    }
    
    if ($message_sent=="yes")
    {
        notify("The e-mail has been sent successfully.");
    }
    
    echo("<h3>Student Information - Placement History</h3>");

    // Default search columns to display.  
    if ((sizeof($searchHistory->column) == 0 || !is_array($searchHistory->column)))
    {
        $searchHistory->column[1]="s.last_name";
        $searchHistory->column[2]="s.first_name";
        $searchHistory->column[3]="IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number";
        $searchHistory->column[4]="t.term_name";
        $searchHistory->column[5]="h.year";
        $searchHistory->column[6]="h.company_name";
        $searchHistory->column[7]="h.company_city";
    }	

    // Default ordering to display.  
    if (!$neworder && (sizeof($searchHistory->order) == 0 || !is_array($searchHistory->order)))
    {
        $searchHistory->order[1]="s.last_name";
        $searchHistory->order[2]="s.first_name";
        $searchHistory->order[3]="work_term_number";
        $searchHistory->order[4]="h.company_name";
    }	
    elseif ($neworder)
    {
        $searchHistory->order[1]=$neworder;
    }

    // Takes care of which columns need to be displayed
    $sql = ("
        SELECT h.history_id, s.email,
        ");
    
    foreach ($searchHistory->column as $x)
    {
        if ($x<>NULL && $x != "s.email")
        {
            $sql .= $x.", ";
        }
    }
    
    $sql = trim($sql);
    if (substr($sql,-1)==",")
    {
        $sql = substr($sql,0,strlen($sql)-1);
    }
    
    $sql .= ("
        FROM student s
        INNER JOIN student_department sd
        ON s.student_number = sd.student_number
        INNER JOIN history h
        ON (h.student_number = sd.student_number) AND (h.department_id = sd.department_id)
        LEFT JOIN history_container hc
        ON h.history_id = hc.history_id 
        ");
    // With the history containers, the work term numbers search is a bit different now, it requires the statement below to handle it properly. 
    if ($searchHistory->work_term_number)
    {
        $sql .= ("
                AND hc.workterm_code REGEXP '^".addslashes(stripslashes($searchHistory->work_term_number))."[a-z]$'
            ");
    }
    
    // Determine what tables to join dynamically. We use flags because we only want to join to a table once. 

    $stuc_flag = 0;     // contact (student)
    $supc_flag = 0;     // contact (supervisor)
    $coopadvc_flag = 0; // contact (co-op advisor)
    $stusvt_flag = 0;   // site_visit_type (student)
    $supsvt_flag = 0;   // site_visit_type (supervisor)
    $divcl_flag = 0;    // country_list (division)
    $stucl_flag = 0;    // country_list (student)
    $divpl_flag = 0;    // provstate_list (division)
    $stupl_flag = 0;    // provstate_list (student)
    $sturl_flag = 0;    // region_list (student)
    $divrl_flag = 0;    // region_list (division)
    
    $d_flag = 0;        // department
    $disc_flag = 0;     // discipline
    $ji_flag = 0;       // job_info
    $hrs_flag = 0;      // history_report_subject
    $hs_flag = 0;       // history_status
    $t_flag = 0;        // term
    $rl_flag = 0;       // region_list
    $sp_flag = 0;       // salary_period


    // This is a minor exception. You can't directly search on a job code with just the history table. So we link it to the job info table.
    if ($searchHistory->job_code)
    {
        $sql .= ("
            LEFT JOIN job_info ji
            ON ji.job_id = h.job_id
            ");
        $ji_flag = 1;
    }


    foreach ($searchHistory->column as $select_column)
    {
        // Everything in $searchHistory->column is of the form (table_alias).(table_column) or (table_alias).(table_column) AS (table_alias). So to extract the
        // table alias, we use the follow regular expression which grabs everything before the '.'. With this table alias, we can determine which tables we need
        // to join with once. If we enter one of the switch cases, a flag is set. This flag informs us that we have already joined with this table, we don't
        // need to join again. 

        $prefix = preg_replace("/(\w+)\..*/","\\1",$select_column);
        switch ($prefix) 
        {
            case "stuc":
                if (!$stuc_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact stuc
                        ON stuc.contact_id = h.site_visit_by
                        ");
                    $stuc_flag = 1;
                }
                break;
            case "supc":
                if (!$supc_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact supc
                        ON supc.contact_id = h.site_visit_by_supervisor
                        ");
                    $supc_flag = 1;
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
            case "stusvt":
                if (!$stusvt_flag)
                {
                    $sql .= ("
                        LEFT JOIN site_visit_type stusvt
                        ON stusvt.site_visit_type_id = h.site_visit_type_id
                        ");
                    $stusvt_flag = 1;
                }
                break;
            case "supsvt":
                if (!$supsvt_flag)
                {
                    $sql .= ("
                        LEFT JOIN site_visit_type supsvt
                        ON supsvt.site_visit_type_id = h.site_visit_type_id_supervisor
                        ");
                    $supsvt_flag = 1;
                }
                break;
            case "divcl":
                if (!$divcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list divcl
                        ON divcl.country_id = h.company_country
                        ");
                    $divcl_flag = 1;
                }
                break;
            case "stucl":
                if (!$stucl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list stucl
                        ON stucl.country_id = h.work_term_country
                        ");
                    $stucl_flag = 1;
                }
                break;
            case "divpl":
                if (!$divpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list divpl
                        ON divpl.provstate_id = h.company_province
                        ");
                    $divpl_flag = 1;
                }
                break;
            case "divrl":
                if (!$divrl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list divrl
                        ON divrl.region_id = h.company_region_id
                        ");
                    $divrl_flag = 1;
                }
                break;
            case "stupl":
                if (!$stupl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list stupl
                        ON stupl.provstate_id = h.work_term_province
                        ");
                    $stupl_flag = 1;
                }
                break;
            case "sturl":
                if (!$sturl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list sturl
                        ON sturl.region_id = h.work_term_region_id
                        ");
                    $sturl_flag = 1;
                }
                break;
            case "d":
                if (!$d_flag)
                {
                    $sql .= ("
                        LEFT JOIN department d
                        ON d.department_id = h.department_id
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
            case "ji":
                if (!$ji_flag)
                {
                    $sql .= ("
                        LEFT JOIN job_info ji
                        ON ji.job_id = h.job_id
                        ");
                    $ji_flag = 1;
                }
                break;
            case "hrs":
                if (!$hrs_flag)
                {
                    $sql .= ("
                        LEFT JOIN history_report_subject hrs
                        ON hrs.report_subject_id = h.report_subject
                        ");
                    $hrs_flag = 1;
                }
                break;
            case "hs":
                if (!$hs_flag)
                {
                    $sql .= ("
                        LEFT JOIN history_status hs
                        ON hs.history_status_id = h.wt_status
                        ");
                    $hs_flag = 1;
                }
                break;
            case "t":
                if (!$t_flag)
                {
                    $sql .= ("
                        LEFT JOIN term t
                        ON t.term_id = h.term_id
                        ");
                    $t_flag = 1;
                }
                break;
            case "sp":
                if (!$sp_flag)
                {
                    $sql .= ("
                        LEFT JOIN salary_period sp 
                        ON h.salary_period_id = sp.salary_period_id
                        ");
                    $sp_flag = 1;
                }
                break;
        }
    }

    unset($prefix);

    // This is the same as above. However, $searchHistory->order contains a list of what we want to order by in the SQL statement. Therefore, we have to join with
    // any table that we want to sort by.  
    foreach ($searchHistory->order as $order_column)
    {
        $prefix = preg_replace("/(\w+)\..*/","\\1",$order_column);
        switch ($prefix) 
        {
            case "stuc":
                if (!$stuc_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact stuc
                        ON stuc.contact_id = h.site_visit_by
                        ");
                    $stuc_flag = 1;
                }
                break;
            case "supc":
                if (!$supc_flag)
                {
                    $sql .= ("
                        LEFT JOIN contact supc
                        ON supc.contact_id = h.site_visit_by_supervisor
                        ");
                    $supc_flag = 1;
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
            case "stusvt":
                if (!$stusvt_flag)
                {
                    $sql .= ("
                        LEFT JOIN site_visit_type stusvt
                        ON stusvt.site_visit_type_id = h.site_visit_type_id
                        ");
                    $stusvt_flag = 1;
                }
                break;
            case "supsvt":
                if (!$supsvt_flag)
                {
                    $sql .= ("
                        LEFT JOIN site_visit_type supsvt
                        ON supsvt.site_visit_type_id = h.site_visit_type_id_supervisor
                        ");
                    $supsvt_flag = 1;
                }
                break;
            case "divcl":
                if (!$divcl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list divcl
                        ON divcl.country_id = h.company_country
                        ");
                    $divcl_flag = 1;
                }
                break;
            case "stucl":
                if (!$stucl_flag)
                {
                    $sql .= ("
                        LEFT JOIN country_list stucl
                        ON stucl.country_id = h.work_term_country
                        ");
                    $stucl_flag = 1;
                }
                break;
            case "divpl":
                if (!$divpl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list divpl
                        ON divpl.provstate_id = h.company_province
                        ");
                    $divpl_flag = 1;
                }
                break;
            case "divrl":
                    if (!$divrl_flag)
                    {
                        $sql .= ("
                            LEFT JOIN region_list divrl
                            ON divrl.region_id = h.company_region_id
                            ");
                        $divrl_flag = 1;
                    }
                    break;
            case "stupl":
                if (!$stupl_flag)
                {
                    $sql .= ("
                        LEFT JOIN provstate_list stupl
                        ON stupl.provstate_id = h.work_term_province
                        ");
                    $stupl_flag = 1;
                }
                break;
            case "sturl":
                if (!$sturl_flag)
                {
                    $sql .= ("
                        LEFT JOIN region_list sturl
                        ON sturl.region_id = h.work_term_region_id
                        ");
                    $sturl_flag = 1;
                }
                break;
            case "d":
                if (!$d_flag)
                {
                    $sql .= ("
                        LEFT JOIN department d
                        ON d.department_id = h.department_id
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
            case "ji":
                if (!$ji_flag)
                {
                    $sql .= ("
                        LEFT JOIN job_info ji
                        ON ji.job_id = h.job_id
                        ");
                    $ji_flag = 1;
                }
                break;
            case "hrs":
                if (!$hrs_flag)
                {
                    $sql .= ("
                        LEFT JOIN history_report_subject hrs
                        ON hrs.report_subject_id = h.report_subject
                        ");
                    $hrs_flag = 1;
                }
                break;
            case "hs":
                if (!$hs_flag)
                {
                    $sql .= ("
                        LEFT JOIN history_status hs
                        ON hs.history_status_id = h.wt_status
                        ");
                    $hs_flag = 1;
                }
                break;
            case "t":
                if (!$t_flag)
                {
                    $sql .= ("
                        LEFT JOIN term t
                        ON t.term_id = h.term_id
                        ");
                    $t_flag = 1;
                }
                break;
            case "sp":
                if (!$sp_flag)
                {
                    $sql .= ("
                        LEFT JOIN salary_period sp 
                        ON h.salary_period_id = sp.salary_period_id
                        ");
                    $sp_flag = 1;
                }
                break;
        }
    }

    // For flags we need to do multiple self joins for each flag the user wants to search for. 
    if (is_array($searchHistory->flags) && sizeof($searchHistory->flags) > 0)
    {
        $i = 0;
        foreach ($searchHistory->flags as $value)
        {
            // An example of what this looks like after the loop is complete. (Assume user searches for 3 flags)

            // INNER JOIN history_flags_join hfj1
            // ON h.history_id = hfj1.history_id // we first link up with history table
            // INNER JOIN history_flags_join hfj2
            // ON hfj1.history_id = hfj2.history_id // then we link the first history_flags_join table with the second, etc.
            // INNER JOIN history_flags_join hfj3
            // ON hfj2.history_id = hfj3.history_id
            $next = $i+1;
            if ($i == 0)
            {
                
                $sql .= ("
                    INNER JOIN history_flags_join hfj".$next."
                    ON h.history_id = hfj".$next.".history_id
                    ");
            }

            else
            {
                $sql .= ("
                    INNER JOIN history_flags_join hfj".$next."
                    ON hfj".$i.".history_id = hfj".$next.".history_id
                    ");
            }
            $i++;
        }
    }
    
    // Begin adding search parameters to the SQL query. 
    $sql .= ("
        WHERE sd.department_id IN ".$department_in_str."
        ");

    // This defines what terms/year we want to search for.  
    if (is_array($searchHistory->termsused) && sizeof($searchHistory->termsused) && (sizeof($searchHistory->term) == sizeof($searchHistory->year)) 
        && $searchHistory->term && $searchHistory->year
        )
    {
        // $flag determines if this we need to start a new "and" statement in SQL. The result of all this will be AND((condition) OR (condition)) 
        $flag = 0;
        // $i is the array index or key.  
        foreach($searchHistory->termsused as $i => $term_value)
        {
            if ($searchHistory->termsused[$i])
            {
                // If we chose to search for Any Term, the value comes back as 1,2,3 (a list of term_ids).
                if (preg_match("/,/",$searchHistory->term[$i]))
                {
                    // split the term_id list into its individual elements. 
                    $temp_term_array = explode(",",$searchHistory->term[$i]);
                    for($j=0;$j<sizeof($temp_term_array);$j++)
                    {
                        if ($flag == 0)
                        {
                            $sql .= ("
                                AND ((h.term_id = '".$temp_term_array[$j]."' AND h.year = '".$searchHistory->year[$i]."')
                                ");
                            $flag++;
                        }

                        else
                        {
                            $sql .= ("
                                OR (h.term_id = '".$temp_term_array[$j]."' AND h.year = '".$searchHistory->year[$i]."')
                                ");
                        }
                       
                    }
                }

                else 
                {
                    if ($flag == 0)
                    {
                        $sql .= ("
                            AND ((h.term_id = '".$searchHistory->term[$i]."' AND h.year = '".$searchHistory->year[$i]."')
                            ");
                        $flag++;
                    }

                    else
                    {
                        $sql .= ("
                            OR (h.term_id = '".$searchHistory->term[$i]."' AND h.year = '".$searchHistory->year[$i]."')
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

    if (strlen(trim($searchHistory->student_num)))
    {
        if ($searchHistory->student_num_search_type == "starts_with")
        {
            $sql .= (" 
                AND sd.student_number LIKE '".addslashes(stripslashes($searchHistory->student_num))."%'
                ");
        }

        elseif ($searchHistory->student_num_search_type == "ends_with")
        {
            $sql .= (" 
                AND sd.student_number LIKE '%".addslashes(stripslashes($searchHistory->student_num))."'
                ");
        }

        else 
        {
            $sql .= (" 
                AND sd.student_number = '".addslashes(stripslashes($searchHistory->student_num))."'
                ");
        }

    }
    
    if ($searchHistory->first_name)
    {
        $sql .= (" 
            AND s.first_name LIKE '".addslashes(stripslashes($searchHistory->first_name))."%' 
            ");
    }
    
    if ($searchHistory->last_name)
    {
        $sql .= (" 
            AND s.last_name LIKE '".addslashes(stripslashes($searchHistory->last_name))."%'
            ");
    }
    
    if ($searchHistory->gender && $searchHistory->gender<>"A")
    {
        $sql .= (" 
            AND s.gender = '".addslashes(stripslashes($searchHistory->gender))."'
            ");
    }
    
    if ($searchHistory->discipline_id)
    {
        $j = 0;
        $flag = 0;
        for($i = 1; $i < (sizeof($searchHistory->discipline_id)); $i++)
        {
            if(($searchHistory->discipline_id[$i]) != "" && ($searchHistory->discipline_id[$i] != NULL))
            {
                $flag = 1;
                $temp_array[$j] = $searchHistory->discipline_id[$i];
                $j++;
            }
    
        }     
        if(($searchHistory->discipline_id[$i]) != "" && ($searchHistory->discipline_id[$i] != NULL))
        {
            $flag =1;
            $temp_array[$j] = $searchHistory->discipline_id[$i];
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

    if ($searchHistory->academic_year)
    {
        $sql .= ("
            AND sd.academic_year = '".addslashes(stripslashes($searchHistory->academic_year))."' 
            ");
    }

    if ($searchHistory->advisor)
    {
        $sql .= ("
            AND sd.advisor = '".addslashes(stripslashes($searchHistory->advisor))."' 
            ");
    }
    
    if ($searchHistory->coop_advisor)
    {
        $sql .= ("
            AND sd.coop_advisor = '".addslashes(stripslashes($searchHistory->coop_advisor))."' 
            ");
    }
    
    if ($searchHistory->citizen)
    {
        $sql .= ("
            AND s.citizen = '".addslashes(stripslashes($searchHistory->citizen))."' 
            ");
    }

    if (strlen(trim($searchHistory->email)))
    {
        if ($searchHistory->email_search_type == "starts_with")
        {
            $sql .= ("
                AND s.email LIKE '".addslashes(stripslashes($searchHistory->email))."%' 
                ");
        }

        elseif ($searchHistory->email_search_type == "contains")
        {
            $sql .= ("
                AND s.email LIKE '%".addslashes(stripslashes($searchHistory->email))."%' 
                ");
        }

        else
        {
            $sql .= ("
                AND s.email = '".addslashes(stripslashes($searchHistory->email))."' 
                ");
        }
    }

    if (strlen(trim($searchHistory->work_email)))
    {
        if ($searchHistory->work_email_search_type == "starts_with")
        {
            $sql .= ("
                AND h.work_email LIKE '".addslashes(stripslashes($searchHistory->work_email))."%' 
                ");
        }

        elseif ($searchHistory->work_email_search_type == "contains")
        {
            $sql .= ("
                AND h.work_email LIKE '%".addslashes(stripslashes($searchHistory->work_email))."%' 
                ");
        }

        else
        {
            $sql .= ("
                AND h.work_email = '".addslashes(stripslashes($searchHistory->work_email))."' 
                ");
        }
    }

    if (strlen(trim($searchHistory->job_code)))
    {
        $sql .= (" 
            AND ji.job_code LIKE '%".addslashes(stripslashes($searchHistory->job_code))."%' 
            ");
    }

    if ($searchHistory->work_term_number)
    {
        $sql .= (" 
            AND (h.work_term_number = '".addslashes(stripslashes($searchHistory->work_term_number))."' OR hc.workterm_code REGEXP '^".addslashes(stripslashes($searchHistory->work_term_number))."[a-z]$')
            ");
    }
    
    if ($searchHistory->work_term_length)
    {
        $sql .= (" 
            AND h.work_term_length = '".addslashes(stripslashes($searchHistory->work_term_length))."'
            ");
    }

    // :TODO: Add salary search parameter when it is fixed. 
    
    if ($searchHistory->wt_status)
    {
        $sql .= (" 
            AND h.wt_status = '".addslashes(stripslashes($searchHistory->wt_status))."' 
            ");
    }

    if ($searchHistory->work_term_city)
    {
        $sql .= (" 
            AND h.work_term_city LIKE '%".addslashes(stripslashes($searchHistory->work_term_city))."%'
            ");
    }

    if ($searchHistory->work_term_province)
    {
        $sql .= (" 
            AND h.work_term_province = '".addslashes(stripslashes($searchHistory->work_term_province))."' 
            ");
    }

    if ($searchHistory->work_term_region)
    {
        $sql .= ("
            AND h.work_term_region_id = '".addslashes(stripslashes($searchHistory->work_term_region))."'
            ");
    }

    if ($searchHistory->work_term_country)
    {
        $sql .= (" 
            AND h.work_term_country = '".addslashes(stripslashes($searchHistory->work_term_country))."' 
            ");
    }

    if ($searchHistory->company_city)
    {
        $sql .= (" 
            AND h.company_city LIKE '%".addslashes(stripslashes($searchHistory->company_city))."%'
            ");
    }

    if ($searchHistory->company_province)
    {
        $sql .= (" 
            AND h.company_province = '".addslashes(stripslashes($searchHistory->company_province))."' 
            ");
    }

    if ($searchHistory->company_region)
    {
        $sql .= ("
            AND h.company_region_id = '".addslashes(stripslashes($searchHistory->company_region))."'
            ");
    }

    if ($searchHistory->company_country)
    {
        $sql .= (" 
            AND h.company_country = '".addslashes(stripslashes($searchHistory->company_country))."' 
            ");
    }
    
    if ($searchHistory->supervisor_name)
    {
        $sql .= (" 
            AND h.supervisor_name LIKE '%".addslashes(stripslashes($searchHistory->supervisor_name))."%'
            ");
    }

    if ($searchHistory->site_visit_date_supervisor && $searchHistory->site_visit_date_supervisor_equality)
    {
        // Ex: AND h.site_visit_date > '2002-11-02'
        $sql .= (" 
            AND h.site_visit_date_supervisor ".addslashes(stripslashes($searchHistory->site_visit_date_supervisor_equality))." '".addslashes(stripslashes($searchHistory->site_visit_date_supervisor))."' 
            ");
    }

    if ($searchHistory->site_visit_by_supervisor)
    {
        $sql .= ("
            AND h.site_visit_by_supervisor = '".addslashes(stripslashes($searchHistory->site_visit_by_supervisor))."' 
            ");
    }

    if ($searchHistory->site_visit_type_id_supervisor)
    {
        $sql .= ("
            AND h.site_visit_type_id_supervisor = '".addslashes(stripslashes($searchHistory->site_visit_type_id_supervisor))."' 
            ");
    }

    if ($searchHistory->site_visit_date && $searchHistory->site_visit_date_equality)
    {
        // Ex: AND h.site_visit_date > '2002-11-02'
        $sql .= (" 
            AND h.site_visit_date ".addslashes(stripslashes($searchHistory->site_visit_date_equality))." '".addslashes(stripslashes($searchHistory->site_visit_date))."' 
            ");
    }

    if ($searchHistory->site_visit_by)
    {
        $sql .= ("
            AND h.site_visit_by = '".addslashes(stripslashes($searchHistory->site_visit_by))."' 
            ");
    }
    
    if ($searchHistory->site_visit_type_id)
    {
        $sql .= ("
            AND h.site_visit_type_id = '".addslashes(stripslashes($searchHistory->site_visit_type_id))."' 
            ");
    }

    if ($searchHistory->report_code)
    {
        $sql .= ("
            AND h.report_code = '".addslashes(stripslashes($searchHistory->report_code))."' 
            ");
    }

    if ($searchHistory->report_subject)
    {
        $sql .= ("
            AND h.report_subject = '".addslashes(stripslashes($searchHistory->report_subject))."' 
            ");
    }

    if ($searchHistory->report_marker)
    {
        $sql .= ("
            AND h.report_marker LIKE '%".addslashes(stripslashes($searchHistory->report_marker))."%' 
            ");
    }

    if ($searchHistory->flags)
    {
        $i=0;
        foreach($searchHistory->flags as $value)
        {
            $next = $i+1;
            $sql .= ("
                AND hfj".$next.".history_flags_id = '".$value."'
                ");
            $i++;
        }
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

        elseif ($neworder == "h.work_term_number")
        {
            $work_term_number_flag = 1;
        }
    }
    
    elseif ($from_student_module=="true")
    {
        $order_by="work_term_number,";
    }

    elseif (sizeof($searchHistory->order) > 0 && is_array($searchHistory->order))
    {
        for($i = 1;$i<=sizeof($searchHistory->order);$i++)
        {
            if ($searchHistory->order[$i]<>NULL)
            {
                $order_by .= $searchHistory->order[$i].",";
               
                if ($searchHistory->order[$i] == "s.last_name")
                {
                    $order_flag = 1;
                }

                elseif ($neworder == "work_term_number")
                {
                    $work_term_number_flag = 1;
                }
            }
        }
    }

    // If we DON'T order by last name, then we tack on s.last_name to the end of the query. Otherwise, we're already searching by last name, so do nothing. 
    if (!$order_flag)
    {
        $order_by .= "s.last_name,";
    }

    // Also if we don't sort by work_term_number, then we do it. 
    if (!$work_term_number_flag)
    {
        $order_by .= "work_term_number,";
    }

    $order_by=substr($order_by,0,strlen($order_by)-1);

    $sql .= " ".$order_by;
    $history_sql = $sql;

    $results = $GLOBALS['dbh']->Execute($history_sql);

    // Pre-count is our initial number of records we get back. 
    $precount = $results->RecordCount();
    $count = 0;

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
            print $i.". ";
            for($j=0;$j<sizeof($arrKeys);$j++)
            {
                print $arrRow[$i][$arrKeys[$j]]."\t";
            }
            print "<br />";
        }
        */
        

        // At this point we have all the information we want stored in an array called $arrRow. If someone wants a list of students that does NOT have certain flags
        // set, we enter this next if clause. This block of code checks if any of these unwanted flags have been set for the history record. If so, we remove that 
        // entry from $arrRow by setting arrRow's history_id to NULL. Afterwards, we copy all records with a non-null history_id to a new arrRow array.

        if(is_array($searchHistory->nflags) && sizeof($searchHistory->nflags) > 0)
        {
            //Algorithm #1: This algorithm works better for small sets of numbers and small sets of flags.
            if (sizeof($searchHistory->nflags) == 1 || ($precount <= 600 && sizeof($searchHistory->nflags) <= 10))
            {
                foreach($searchHistory->nflags as $flag_id)
                {
                    for($i=0;$i<sizeof($arrRow);$i++)
                    {
                        if ($arrRow[$i]['history_id'])
                        {
                            $flag_sql = ("
                                SELECT h.history_id
                                FROM history h
                                INNER JOIN history_flags_join hfj
                                ON h.history_id = hfj.history_id
                                WHERE hfj.history_flags_id = '".$flag_id."' AND h.history_id = '".$arrRow[$i]['history_id']."'
                                ");
                            $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                            if ($flag_result->RecordCount() > 0)
                            {
                                $arrRow[$i]['history_id'] = NULL;
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
                foreach($searchHistory->nflags as $flag_id)
                {
                    if ($i==0)
                    {
                        $conditions .= "hfj.history_flags_id = '".$flag_id."' ";
                        $i++;
                    }

                    else
                    {
                        $conditions .= "OR hfj.history_flags_id = '".$flag_id."' ";
                    }
                }

                $conditions = "(".$conditions.")";

                for($i=0;$i<sizeof($arrRow);$i++)
                {
                    if ($arrRow[$i]['history_id'])
                    {
                        $flag_sql = ("
                                SELECT h.history_id
                                FROM history h
                                INNER JOIN history_flags_join hfj
                                ON h.history_id = hfj.history_id
                                WHERE hfj.history_flags_id = '".$flag_id."' AND ".$conditions."
                                ");
                        $flag_result = $GLOBALS['dbh']->Execute($flag_sql);

                        if ($flag_result->RecordCount() > 0)
                        {
                            $arrRow[$i]['history_id'] = NULL;
                        }
                    }
                }
            }
            // End Algorithm #2

            // Rebuild array. 
            $tmpArrRow = array();
            $counter = 0;
            for ($i=0;$i<sizeof($arrRow);$i++)
            {
                if ($arrRow[$i]['history_id'] != NULL)
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
        }
        
        $count = sizeof($arrRow);
    }
    // That's the end of building the SQL
    // Start the presentation

    // quick navigation buttons
    echo("<table border='0' cellspacing='0' cellpadding='0'>");
    echo("<tr>");
    echo("<td>");
    echo("<form method='post' action='".$PHP_SELF."&amp;select=history_choose'>");
    echo("<input type='hidden' name='searchmode' value='advanced' />");
    echo("<input type='submit' value='New Search' />");
    echo("&nbsp;");
    echo("</form>");
    echo("</td>");
    echo("<td>");
    echo("<form method='post' action='".$PHP_SELF."&amp;select=history_choose'>");
    echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
    echo("<input type='hidden' name='searchmode' value='advanced' />");
    echo("<input type='submit' name='btnSearch' value='Edit Search Criteria' />");
    echo("&nbsp;");
    echo("</form>");
    echo("</td>");
    echo("<td>");
    //if this search is not saved yet you can still save it 
    if(!$search_saved) {
        echo("<form name='save_search_form' action='".$PHP_SELF."&amp;select=history_advanced_search' method='post'>");
        echo("<input type='hidden' name='searchmode' value='advanced' />");
        echo("<input type='hidden' name='validate_form' value='1' />");
        echo("<input type='hidden' name='searchHistory' value='" . packObject($searchHistory) . "' />");
        echo("<input type='submit' name='btnSearch' value='Save Search' />");
        echo("&nbsp;");
        echo("</form>");
    }

    echo("</td>");
    echo("</tr>");
    echo("</table>");

    if ($count > 0)
    {
        // Build Student List for 'Next' and 'Previous'
        $history_list=NULL;

        for($i=0;$i<sizeof($arrRow);$i++)
        {
            $history_list .= $arrRow[$i]['history_id'].",";
        }

        $history_list = substr($history_list, 0, -1);

        // Paging begins. 
        if ($per_page_max == "") { $per_page_max = 50; }
        if ($per_page_max < 5) { $per_page_max = 5; }
        $pages = ceil($count / $per_page_max);

        if ($count>$per_page_max)
        {
            echo("<table border='0' cellspacing='0' cellpadding='4' class='row1'>");
                echo("<tr>");
                    echo("<td>");

            echo("<form action='".$PHP_SELF."' method='post'>");
            echo("<input type='hidden' name='btnSearch' value='Search' />");
            echo("<input type='hidden' name='select' value='history_advanced_search' />");
            echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");

            echo("<table border='0' cellspacing='0' cellpadding='4' class='row1'>");
                echo("<tr>");
                    echo("<td>");
                        echo($count." record" . (($count != 1) ? "s" : "") . " on ".$pages." page" . (($pages > 1) ? "s" : "") . ";<br />"); 
                    echo("</td>");
                    echo("<td>");
                        echo("<input type='text' name='per_page_max' size='4' maxlength='4' value='".$per_page_max."' /> records per page.");
                    echo("</td>");
                echo("</tr>");

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

                        // By we list the the first order field in the dropdown menu. 
                        else
                        {
                            $display_column = preg_replace("/(.*)\.(.*)/","\\2",$searchHistory->order[1]);
                            $display_column = trim($display_column);
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
                    echo("</td>");
                    echo("<input type='hidden' name='advance' value='TRUE' />");
                    echo("<input type='hidden' name='neworder' value='".$neworder."' />");
                    if ($neworder_display)
                    {
                        echo("<input type='hidden' name='newvalue' value='".$newvalue."' />");
                    }

                    echo("<td align='right'>");
                        echo("<input type='submit' value='View' />");
                    echo("</td>");
                echo("</tr>");
                echo("</table>");

                echo("</form>");
                echo("</td>");
                echo("</tr>");
                echo("</table>");
        } 
        
        else 
        {
            echo("<h4>".$count." records:</h4>");
        }

        if ($start_row == '') { $start_row = 0; }
        $first = $start_row + 1;
        $end = $start_row + $per_page_max;
        if ($end > $count) { $end = $count; }

        echo("<form method='post' action='" . $PHP_SELF . "' name='student_form'>");
        echo("<table cellspacing='0' border='1' cellpadding='0'>");
            echo("<tr>");
                echo("<td>");
                    echo("<table cellpadding='2' border='0'>");
                        echo("<tr class='rowgrey'>");
                            echo("<td>&nbsp;</td>");

                            // form string is used to capture the construction of an HTML form, and echoing it
                            // later because nested forms aren't allowed
                            $form_string = null;

                            // Display column titles. 
                            for ($i=1;$i<=sizeof($searchHistory->column);$i++)
                            { 
                                $value = $searchHistory->column[$i];

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
                                    
                                    echo("<td align='center' nowrap='nowrap'>");

                                    ob_start();
                                    echo("<form name='form".$i."' action='".$PHP_SELF."' method='post'>");
                                        echo("<input type='hidden' name='searchmode' value='advanced' />");
                                        echo("<input type='hidden' name='select' value='history_choose' />");
                                        echo("<input type='hidden' name='btnSearch' value='Search' />");
                                        echo("<input type='hidden' name='searchHistory' value='".packObject($searchHistory)."' />");
                                        echo("<input type='hidden' name='newvalue' value='".$columns_array[$matched_index]['value']."' />");
                                        echo("<input type='hidden' name='neworder' value='".$columns_array[$matched_index]['order']."' />");
                                    echo("</form>");
                                    $form_string .= ob_get_contents();
                                    ob_end_clean();

                                    echo("<a href='javascript:document.form".$i.".submit()'>&nbsp;<b class='white'>".$header."</b>&nbsp;</a>");

                                    echo("</td>");

                                    $export_string .= $header."\t";
                                    if ($header == "Salary")
                                    {
                                        $export_string .= "Salary Period\t";
                                    }
                                }
                                unset($header);
                            }

                            // Export to file and email address list creation. 
                            for($i=0;$i<sizeof($arrRow);$i++)
                            {
                                $export_string .= "\n";

                                $temp_ex = "";
                                $matched_flag = 0;
                                for ($j=1;$j<=sizeof($searchHistory->column);$j++)
                                {
                                    $value = $searchHistory->column[$j];
                                    if ($value<>NULL)
                                    {
                                       //working 
                                        // Special formatting cases. 
                                        switch ($value)
                                        {
                                            case "h.company_street_address_1,h.company_street_address_2":
                                                $temp_ex = $arrRow[$i]["company_street_address_1"]."  ".$arrRow[$i]["company_street_address_2"];
                                                $matched_flag = 1;
                                                break;
                                            case "stuc.first_name AS stuc_first_name,stuc.last_name AS stuc_last_name":
                                                $temp_ex = $arrRow[$i]["stuc_first_name"]." ".$arrRow[$i]["stuc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name":
                                                $temp_ex = $arrRow[$i]["coopadvc_first_name"]." ".$arrRow[$i]["coopadvc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "h.work_term_street_address_1,h.work_term_street_address_2":
                                                $temp_ex = $arrRow[$i]["work_term_street_address_1"]." ".$arrRow[$i]["work_term_street_address_2"];
                                                $matched_flag = 1;
                                                break;
                                            case "supc.first_name AS supc_first_name,supc.last_name AS supc_last_name":
                                                $temp_ex = $arrRow[$i]["supc_first_name"]." ".$arrRow[$i]["supc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "sp.salary_period_name, h.salary":
                                                $temp_ex = $arrRow[$i]["salary"]."\t".$arrRow[$i]["salary_period_name"];
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
                            } // end for

                        $bcc = trim($bcc);

                        if (substr($bcc,-1)==",")
                        {
                            $bcc = substr($bcc,0,strlen($bcc)-1);
                        }

                        echo("</tr>");

                        $colour = 0;
                        for($i=$first-1;$i<$end;$i++)
                        {
                            $colour++;
                            $class = ($colour%2 == 0) ? "row0d" : "row1d";

                            echo("<tr>");
                                echo("<td align='center' class='" . $class . "' nowrap='nowrap'>");
                                    echo("&nbsp;");
                                    echo("<input type='radio' ".(($colour == 1) ? "checked='checked'" : "")." name='history_id' value='".$arrRow[$i]["history_id"]."' onclick='submit()' />");
                                    echo("&nbsp;");
                                echo("</td>");

                                unset($outstring);
                                unset($matched_flag);
                                unset($match_string_1);
                                unset($match_string_2);
                                $matched_flag = 0;

                                // Display search results. The structure is almost the same as export to file. Refer above for more details.  
                                for($j=1;$j<=sizeof($searchHistory->column);$j++)
                                {
                                    $value = $searchHistory->column[$j];
                                    if ($value<>NULL)
                                    {
                                        switch ($value)
                                        {
                                            case "h.company_street_address_1,h.company_street_address_2":
                                                $outstring = $arrRow[$i]["company_street_address_1"]."  ".$arrRow[$i]["company_street_address_2"];
                                                $matched_flag = 1;
                                                break;
                                            case "stuc.first_name AS stuc_first_name,stuc.last_name AS stuc_last_name":
                                                $outstring = $arrRow[$i]["stuc_first_name"]." ".$arrRow[$i]["stuc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name":
                                                $outstring = $arrRow[$i]["coopadvc_first_name"]." ".$arrRow[$i]["coopadvc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "h.work_term_street_address_1,h.work_term_street_address_2":
                                                $outstring = $arrRow[$i]["work_term_street_address_1"]." ".$arrRow[$i]["work_term_street_address_2"];
                                                $matched_flag = 1;
                                                break;
                                            case "supc.first_name AS supc_first_name,supc.last_name AS supc_last_name":
                                                $outstring = $arrRow[$i]["supc_first_name"]." ".$arrRow[$i]["supc_last_name"];
                                                $matched_flag = 1;
                                                break;
                                            case "s.email":
                                                $outstring = "<FONT size='-1'><a href='mailto:" . $arrRow[$i]['email'] . "'>&nbsp;";
                                                $outstring .= $arrRow[$i]['email'] . "&nbsp;</a></FONT>";
                                                $matched_flag = 1;
                                                break;
                                            case "h.work_email":
                                                $outstring = "<FONT size='-1'><a href='mailto:" . $arrRow[$i]['work_email'] . "'>&nbsp;";
                                                $outstring .= $arrRow[$i]['work_email'] . "&nbsp;</a></FONT>";
                                                $matched_flag = 1;
                                                break;
                                            case "sp.salary_period_name, h.salary":
                                                $outstring = $arrRow[$i]["salary"];
                                                // If a salary exists, append /salary period to it.
                                                if ($outstring)
                                                {
                                                    $outstring .= "/".$arrRow[$i]["salary_period_name"];
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
                            echo("</tr>");
                        } // end while

                        $column_count=0;
                        foreach ($searchHistory->column as $column_row)
                        {
                            if ($column_row<>NULL)
                            {
                                $column_count=$column_count+1;
                            }
                        }


                        echo("<tr class='" . ($class == "row0d" ? "row1d" : "row0d") . "'>");
                            echo("<td colspan='".($column_count+1)."'>");
                                echo("<input type='hidden' name='student_num' value='" . $student_num . "' />");
                                echo("<input type='hidden' name='student_list' value='" . $student_list . "' />");
                                echo("<input type='hidden' name='student_department' value='" . $student_department . "' />");
                                echo("<input type='hidden' name='searchStudents' value='" . $searchStudents . "' />");
                                echo("<input type='hidden' name='searchHistory' value='" . packObject($searchHistory) . "' />");
                                // The following two variables are used for paging purposes. 
                                echo("<input type='hidden' name='start_row' value='" . $start_row . "' />");
                                echo("<input type='hidden' name='per_page_max' value='" . $per_page_max . "' />");
                                echo("<input type='hidden' name='select' value='history' />");
                                echo("<input type='hidden' name='history_list' value='" . $history_list . "' />");
                                echo("<input type='submit' value='View Selected Student' /> &nbsp; ");
                                echo("<input type='hidden' name='export' value='' />");
                                echo("<input type='hidden' name='export_string' value='". packObject($export_string) ."' />");
                            echo("</td>");
                        echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");
        echo("</table>");
        echo("</form>");
        ?>

        <?php
       
        echo("<table border='0' cellspacing='0' cellpadding='0'>");
        echo("<tr>");
        echo("<td>");
        echo("<input type='button' value=' Export to File ' onclick='javascript:triggerExport(\"student_form\", \"export\");' /> ");
        echo("<input type='button' value='Export Records to PDF' onclick='javascript:document.export_batch_pdf_form.submit()' /> ");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td>");
        echo("&nbsp;");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td>");
        echo("<input type='button' value='E-mail All Matches' onclick='javascript:document.email_form.submit()' /> ");
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

        echo("<form name='email_form' method='post' action='" . $PHP_SELF . "&amp;select=history_email_students'>");
        echo("<input type='hidden' name='bcc' value='" . $bcc . "' />");
        echo("<input type='hidden' name='searchHistory' value='" . packObject($searchHistory) . "' />");
        echo("<input type='hidden' name='students_without_email' value='" . packObject($students_without_email) . "' />");
        echo("</form>");

        // PDF batch stuff
        echo("<form name='export_batch_pdf_form' method='post' action='".$PHP_SELF."&amp;select=export_pdf_batch'>");
        echo("<input type='hidden' name='history_id_list' value='".urlencode($history_list)."' />");
        echo("</form>");

    }
    else
    {
        echo("<table>");
            echo("<tr>");
                echo("<td>&nbsp;</td>");
            echo("</tr>");
            echo("<tr>");
                notify("No matches found.");
            echo("</tr>");
        echo("</table>");
    }

    echo($form_string);
?>

<script type='text/javascript'>
<!--javascript

function triggerExport(form_name, element_name) {

    form = eval("document." + form_name); 
    flag = 0;
    var i;
    for (i = 0; i < form.elements.length; i++) {
        if (form.elements[i].name == element_name) {
            form.elements[i].value = 1;
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
