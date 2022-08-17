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
 | Filename: MamookReportJobDevelopment.class.php                               |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


if (IN_MAMOOK !== true) {
    header("HTTP/1.0 404 Not Found");
    die();
}

class MamookReportJobDevelopment extends PluginInterface {

    function MamookReportJobDevelopment() {
        $this->plugin_version_number = '1.0';
        $this->plugin_author = 'University of Victoria';
        $this->plugin_name = 'Job Development Report';
        $this->plugin_type = 'contact_search_results_plugin';
    }

    function init() {
        register_hook("mamook.traffic_control.select", $this, "handleTraffic");
        register_hook("mamook.contact_search_results.button", $this, "getButtonAndForm");
    }

    function handleTraffic($select) {
        global $auth;
        global $PHP_SELF;

        $plugin_absolute_path = $this->getPluginAbsolutePath();

        // temporary work around to local scoping of variables
        if (ini_get('register_globals')) {
            include('misc/compatibility.inc');
        }

        switch ($select) {
            case "job_development_report_form":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR) {
                    include($plugin_absolute_path.'main.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }

            case "job_development_report":
                if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR) {
                    include($plugin_absolute_path.'job_development.inc');
                    return true;
                }
                else {
                    include('misc/loguser.inc');
                    return null;
                }
            default:
                return null;
        }
    }

    // $arr_params['id_list'] is the $id_list
    // $arr_params['id_list_name'] the name of this list value in the form 
    // $arr_params['PHP_SELF'] is $PHP_SELF
    function getButtonAndForm($arr_params) {
        $arr_return['button'] = "<input type='button' value='Job Development Report' onclick='javascript:document.job_dev_report_form.submit()' />";

        $string = ("<form method='post' action='".$arr_params['PHP_SELF']."&amp;select=job_development_report_form' name='job_dev_report_form'>");                       
        $string .= ("<input type='hidden' name='contact_id_string' value='".packObject($arr_params['contact_id_list'])."' />");                                           
        $string .= ("</form>");

        $arr_return['form'] = $string;

        return $arr_return;
    }
}

?>
