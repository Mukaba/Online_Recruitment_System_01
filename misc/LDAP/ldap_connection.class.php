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
 | Filename: ldap_connection.class.php                                          |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


class ldap_connection {
    var $host;
    var $port;
    var $access_dn;
    var $ds;

    function ldap_connection($host, $access_dn, $port = null) {
        $this->host = $host;
        $this->port = $port;
        $this->access_dn = $access_dn;

        $this->connect();
    }

    // convenience wrap methods
    function connect() {
        $this->ds = @ldap_connect($this->host, $this->port);
    }

    function bind($dn = null, $password = null) {
        return @ldap_bind($this->ds, $dn, $password);
    }

    function search($dn, $filter) {
        return @ldap_search($this->ds, $dn, $filter);
    }

    function get_entries($result) {
        return @ldap_get_entries($this->ds, $result);
    }

    // debugging methods
    function to_string() {
        $str = "host: $this->host:$this->port\n";
        $str .= "access dn: $this->access_dn\n";
        return $str;
    }

    function dump_object() {
        echo $this->to_string();
    }
}

?>
