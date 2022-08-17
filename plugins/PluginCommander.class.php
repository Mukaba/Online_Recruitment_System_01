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
| Filename: PluginCommander.class.inc                                          |
+------------------------------------------------------------------------------+
*/

include('plugins/PluginInterface.class.php');
/** 
 * This class holds all plugin information, and should be the only data path to the plugin table.
 * Use this to load/install/remove plugins from the system.
 * @author Patrick Wilkerson <patrick@mamook.net>
 */
 
class PluginCommander {
    /**
     * @var array $plugins This holds all the plugin objects that are loaded in the system.
     */
    var $plugins = array();

    /**
     * @var array $loaded_plugins This holds the actual instances of the plugins, not the placeholders like $plugins
     */
    var $loaded_plugins = array();

    /**
     * @var array $hooks This is the array of hooks that are registered.
     */
    var $hooks = array();
    
    /**
     * Constructor.  Automatically calls getPlugins on creation.
     */
    function PluginCommander(){
        $this->plugins = $this->getPlugins();
        $plugin_dirs = explode(":", PATH_PLUGIN_DIR);
        if (count($plugin_dirs) > 0){
            foreach ($plugin_dirs as $dir){
                $this->checkForNewPlugins($dir);
            }
        }
        $this->plugins = $this->getPlugins(); //get the new ones we found
    }

    /**
     * This loads the list of plugins that are currently available to the system.
     * @private
     */
    function &getPlugins(){
        $sql = "SELECT plugin_id
                FROM plugins";
        $result = $GLOBALS['dbh']->Execute($sql);
        if ($result->RecordCount() == 0){
            return array();
        }
        $plugins = array();
        while ($row = $result->FetchRow()){
            $plugins[] = new Plugin($row['plugin_id']);
        }
        return $plugins;
    }

    /**
     * This returns the array of available plugin objects
     */
    function getAvailablePlugins(){
        return $this->plugins;
    }

    /**
     * Checks to see if a plugin is available to be used/loaded
     * @param string $plugin_name The name of the plugin to check for
     * @return boolean 
     */
    function isPluginAvailable($plugin_path){
        if (sizeof($this->plugins) == 0) return false;
        for ($i = 0; $i < count($this->plugins); $i++){
            if (strtolower(trim($plugin_path)) == strtolower(trim($this->plugins[$i]->plugin_path))) return true;
        }
        return false;
    }

    /**
     * Scan the dir for new plugins to load
     */
    function checkForNewPlugins($dir){
        if (!file_exists($dir)) return trigger_error("Plugin Scan Failed: $dir does not exist.");
        $dirs = array();
        if ($root = opendir($dir)){
            while ($file = readdir($root)){
                if(is_dir($dir.$file) && $file != "." && $file != ".."){
                    $dirs[] = $file;
                }
            }
            sort($dirs);
            foreach ($dirs as $plugin_path){
                if (!$this->isPluginAvailable($plugin_path) && file_exists($dir.$plugin_path."/$plugin_path.class.php")){
                    include_once($dir.$plugin_path."/$plugin_path.class.php");
                    $new_plugin = new $plugin_path();
                    
                    $name = $new_plugin->getPluginName(); 
                    if (empty($name)){
                        trigger_error("Install Plugin Failed: Plugin '$plugin_path' has no name."); 
                    } unset($name);
                    
                    $type = $new_plugin->getPluginType();
                    if (empty($type)){
                        trigger_error("Install Plugin Failed: Plugin '$plugin_path' has no type."); 
                    } unset($type);

                    $new_plugin_insert = $GLOBALS['dbh']->Prepare("INSERT INTO plugins (plugin_name, plugin_type, plugin_path) VALUES (?,?,?)");
                    $GLOBALS['dbh']->Execute($new_plugin_insert, array($new_plugin->getPluginName(), $new_plugin->GetPluginType(), $plugin_path));
                    $new_plugin->install();
                }
            }
        }
    }

    function loadPluginsByType($plugin_type){
        for ($i = 0; $i < count($this->plugins); $i++){ 
            if ($plugin_type == "*" || strtolower(trim($this->plugins[$i]->plugin_type)) == strtolower(trim($plugin_type))){
                PluginCommander::loadPlugin($this->plugins[$i]->plugin_path);
            }
        }
    }
    
    /*
     * @static
     */
    function loadPlugin($plugin_path){
        if (!$this->isPluginAvailable($plugin_path)) return;
        $plugin_paths = explode(":", PATH_PLUGIN_DIR);
        foreach ($plugin_paths as $dir){
            $plugin_abspath = $dir . $plugin_path . "/$plugin_path.class.php";
            if (file_exists($plugin_abspath)){
                include_once($plugin_abspath);
                if (!class_exists($plugin_path)) continue; // If no class, go to next iteration
                $plugin = new $plugin_path();
                $plugin->plugin_path = $plugin_path;
                $plugin->plugin_absolute_path = $dir . $plugin_path .'/';
                $plugin->init();
                $this->loaded_plugins[$plugin->getPluginPath()] =& $plugin;
            }
        }
    }
    /**
     * Register a hook.
     * @param string $hookname The name of the hook to register under.
     * @param object PluginInterface $plugin The plugin itself, for identification purposes.  Pass in $this when you are calling it.
     * @param string $plugin_method The method within the plugin to call with the paramaters at get_hooks time.
     */
    function register_hook($hookname, &$plugin, $plugin_method){
        $this->hooks[strtolower(trim($hookname))][$plugin->getPluginPath()] = $plugin_method;
    }

    /**
     * This is the function called when you want to get responses from the various registered hooks.
     * @param string $hook The name of the hook (registered with register_hook) to call.
     * @param mixed $params This can be anything that you want sent to the hook.  This will be dependant on each situation.
     * @return array The array of responses from the plugins.
     */
    function get_hooks($hook, $params){
        $hook_return = array();
        $hook = strtolower(trim($hook));
        if (!isset($this->hooks[$hook]) || sizeof($this->hooks[$hook]) == 0){
            return $hook_return;
        } else {
            foreach ($this->hooks[$hook] as $plugin_name => $plugin_method){
                $return = $this->loaded_plugins[$plugin_name]->$plugin_method($params);
                if (!is_null($return)) $hook_return[] = $return; 
            }
        }
        return $hook_return;
    }
}

/**
 * Container, not that useful for anything other than PluginCommander
 * @author Patrick Wilkerson <patrick@mamook.net>
 */
class Plugin {
    var $plugin_path;
    var $plugin_type;
    var $plugin_id;

    function Plugin($id = -1){
        if ($id == -1) return;
        $result = $GLOBALS['dbh']->Execute("SELECT * FROM plugins WHERE plugin_id = $id");
        $row = $result->FetchRow();
        $this->plugin_path = $row['plugin_path'];
        $this->plugin_type = $row['plugin_type'];
        $this->plugin_id = $id;
    }
}
?>
