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
| Filename: PluginInterface.class.php                                          |
+------------------------------------------------------------------------------+
| Description: The main plugin interface that all plugins need to extend from. |
+------------------------------------------------------------------------------+
*/ 

class PluginInterface {
    var $plugin_version_number;
    var $plugin_author;
    var $plugin_name;
    var $plugin_type;
    var $plugin_dependencies;
    var $plugin_path;
    var $plugin_absolute_path; 

    function getPluginPath(){
        return $this->plugin_path;
    }
    function init() {
        // you implement this
    }
    function install() {
        // you implement this
    }
    function getVersion() {
        return $this->plugin_version;
    }
    function getAuthor() {
        return $this->plugin_author;
    }
    function getPluginName() {
        return $this->plugin_name;
    }
    function getPluginType() {
        return $this->plugin_type;
    }
    function getDependencies() {
        return $this->plugin_dependencies;
    }
    function getPluginAbsolutePath() {
        return $this->plugin_absolute_path;
    }
}
?>
