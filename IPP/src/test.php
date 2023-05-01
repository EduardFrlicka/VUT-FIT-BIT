#! /usr/bin/php
<?php

//commmon return values
define("ERR_ARG", 10);  // - chybějící parametr skriptu (je-li třeba) nebo použití zakázané kombinace parametrů;
define("ERR_IN", 11);   // - chyba při otevírání vstupních souborů (např. neexistence, nedostatečné oprávnění);
define("ERR_OUT", 12);  // - chyba při otevření výstupních souborů pro zápis (např. nedostatečné oprávnění, chyba při zápisu);

//script specific return value
define("ERR_ARG_PATH", 41);     // - zadaný adresář (path v parametru --directory) nebo zadaný soubor (file v parametrech --parse-script, --int-script a --jexampath) neexistuje či není přístupný

//internal error return value
define("ERR_INTERNAL", 99);     // - interní chyba (neovlivněná vstupními soubory či parametry příkazové řádky; např. chyba alokace paměti).

//test return codes
define("TEST_PASS", 0);
define("TEST_FAIL", 1);
define("TEST_SKIP", 2);

//general return codes
define("FAIL", false);
define("PASS", true);


// default paths
$parse_path = "parse.php";
$interpret_path = "interpret.py";
$tests_dir = ".";
$jexam_path = "/pub/courses/ipp/jexamxml/";

//diffs
$jexam_diff = '$java_exec $jexam_path';
$diff = "diff";

// exec paths
$php_exec = "php8.1";
$java_exec = "java -jar";
$python_exec = "python3";

// initialization
$cnt = 0;
$passed = 0;
$clean = true;
$recursive = false;
$test_function = "test_both";
$passed_tests = [];
$failed_tests = [];

// $verbose = false;
$tick = "progress_bar";
$tty = stream_isatty(STDERR);

// $stderr = "";

$stderr = "2>/dev/null";


// ARGUMENT PARSING
$options = getopt("hvq", array("help", "directory:", "recursive", "parse-script:", "int-script:", "parse-only", "int-only", "jexampath:", "noclean", "verbose", "quiet", "testlist"));

if (array_key_exists("h", $options) || array_key_exists("help", $options)) {
    var_dump($options);
    exit(0);
}

if (array_key_exists("v", $options) || array_key_exists("verbose", $options)) {
    $verbose = true;
}

if (array_key_exists("q", $options) || array_key_exists("quiet", $options)) {
}


if (array_key_exists("directory", $options)) {
    $tests_dir = $options["directory"];
    if (!is_dir($tests_dir))
        error(ERR_ARG_PATH, "Cannot find or open %s directory", $tests_dir);
}

if (array_key_exists("parse-script", $options)) {
    $parse_path = $options["parse-script"];
    if (!is_executable($parse_path))
        error(ERR_ARG_PATH, "Cannot find or open %s path", $parse_path);
}

if (array_key_exists("int-script", $options)) {
    $interpret_path = $options["int-script"];
    if (!is_executable($interpret_path))
        error(ERR_ARG_PATH, "Cannot find or open %s path", $interpret_path);
}

if (array_key_exists("jexampath", $options)) {
    $jexam_path = $options["jexampath"];
    if (!is_dir($jexam_path))
        error(ERR_ARG_PATH, "Cannot find or open %s directory", $jexam_path);
}

if (array_key_exists("recursive", $options)) {
    $recursive = true;
}

if (array_key_exists("noclean", $options)) {
    $clean = false;
}

if (array_key_exists("parse-only", $options)) {
    if (array_key_exists("int-only", $options) || array_key_exists("int-script", $options))
        error(ERR_ARG, "cannot combine parse-only with interpret operator");

    $test_function = "test_parse";
}

if (array_key_exists("int-only", $options)) {
    if (array_key_exists("parse-only", $options) || array_key_exists("parse-script", $options) || array_key_exists("jexampath", $options))
        error(ERR_ARG, "cannot combine int-only with parser operator");

    $test_function = "test_interpret";
}

if (array_key_exists("testlist", $options) && array_key_exists("directory", $options))
    error(ERR_ARG, "cannot combine testlist and directory");

if (array_key_exists("match", $options)) {
    $regex = $options["regex"];
    if (preg_match($regex, '') === false)
        error(ERR_IN, "worng regex");
}


// jexam diff eval
$jexam_options = $jexam_path . DIRECTORY_SEPARATOR . "options";
$jexam_path .= DIRECTORY_SEPARATOR . "jexamxml.jar";
eval("\$jexam_diff = \"$jexam_diff\";");

// All tests count
$command = "find " . $tests_dir . " -type f -name \"*.src\"";
$command .= " | wc -l";

exec($command, $output, $rc);
if ($rc != 0)
    error(ERR_INTERNAL, "Failed exec call %s", $command);
$all_tests = (int)$output[0];



// INITIALIZATION
$bar = new ProgressBar($all_tests);

// TESTING
if (array_key_exists("testlist", $options)) {
    $testlist_file = $options["testlist"];
    if (!is_readable($testlist_file))
        error(ERR_ARG_PATH, "Cannot find or open %s path", $testlist_file);

    $testlist = file($testlist_file);
    foreach ($testlist as $path) {
        if (is_dir($path)) {
            TestDirContents($path, $test_function);
        } else {
            $test_path = preg_replace("/\.src$/", "", $path);

            list($res, $expr_rc, $rc, $err_msg) = execute_test($test_function, $test_path);

            if ($res == TEST_PASS)
                $passed++;
            else
                    if ($verbose) fprintf(STDERR, "%s, %s\n", "FAIL", $path);

            print_test($res, $test_path, $expr_rc, $rc, $err_msg);
            $bar->tick();
        }
    }
} else {
    testDirContents($tests_dir, $test_function);
}


class ProgressBar
{
    private int $cols = 0;
    private int $all = 0;
    private int $cnt = 0;

    private int $fill_chars = 0;
    private int $blank_chars = 0;

    private string $blank = '-';
    private string $fill = '#';
    private string $prefix = " [ ";
    private string $suffix = "";

    const STREAM = STDERR;

    public function __construct($all)
    {
        $this->all = $all;

        // Cols count
        $command = "tput cols";
        exec($command, $output, $rc);
        if ($rc != 0)
            error(ERR_INTERNAL, "Failed exec call %s", $command);

        $this->cols = (int)$output[0];
        unset($output);
    }

    function tick()
    {
        // Math
        $this->cnt++;
        $proportion = $this->cnt / $this->all;

        // suffix calculation
        $suffix = sprintf(" ] %04.1f%%", $proportion * 100);

        // Other Math
        $progress_bar_len = $this->cols - strlen($this->prefix) - strlen($suffix);

        $fill_chars = (int)($proportion * $progress_bar_len);
        $blank_chars = $progress_bar_len - $fill_chars;


        // printing just if some value changed
        if ($this->fill_chars != $fill_chars || $this->blank_chars != $blank_chars || $this->suffix != $suffix) {
            // printing to stderr
            fprintf(self::STREAM, "\r%s%s%s%s", $this->prefix, str_repeat($this->fill, $fill_chars), str_repeat($this->blank, $blank_chars), $suffix);

            // saving new values
            $this->fill_chars = $fill_chars;
            $this->blank_chars = $blank_chars;
            $this->suffix = $suffix;
        }
    }

    function __destruct()
    {
        fprintf(self::STREAM, "\n");
    }
}

function error($code, $fmt, ...$args)
{
    vfprintf(STDERR, $fmt . "\n", $args);
    die($code);
}

function warning($fmt, ...$args)
{
    vfprintf(STDERR, $fmt . "\n", $args);
}

function test_input_file($name)
{
    return is_readable($name);
}

function test_output_file($name)
{
    return !file_exists($name) || is_writable($name);
}

function ensure_input_file($file_name, $default_text)
{
    if (!file_exists($file_name)) {    // if file does not exist, create default one
        $file = fopen($file_name, "w");

        if ($file == false)
            error(ERR_IN, "Insufficient perrmisions to create file %s", $file_name);

        fwrite($file, $default_text);
        fclose($file);
    }

    if (!test_input_file($file_name))    // checking for permissions
        error(ERR_IN, "Insufficient perrmisions to read file %s", $file_name);
}

function execute_test($test_function, $test_path)
{
    // test files paths
    $src_file = "$test_path.src";
    $ref_file = "$test_path.out";
    $in_file = "$test_path.in";
    $rc_file = "$test_path.rc";
    $out_file = "$test_path.myout";

    // checking and creating default file in case of missing file

    // src should exist (in recursive search looking for *.src files), just checking permissions
    if (!test_input_file($src_file))
        error(ERR_IN, "Insufficient perrmisions to read file %s", $src_file);

    // other files
    ensure_input_file($ref_file, "");
    ensure_input_file($in_file, "");
    ensure_input_file($rc_file, "0");

    // TEST EXECUTION
    return $test_function($src_file, $ref_file, $in_file, $rc_file, $out_file);
}

function test_parse($src_file, $ref_file, $in_file, $rc_file, $out_file)
{
    global $php_exec, $parse_path, $jexam_diff, $clean, $stderr, $jexam_options;

    // executing parse script
    $command = "$php_exec $parse_path <$src_file >$out_file $stderr";
    $retval = execute_command($command);

    // reading expected return value
    $exp_retval = (int) file_get_contents($rc_file);

    // comparing return values
    if ($retval != 0)
        return array($exp_retval == $retval ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Return code");

    // executing diff
    $diff_command = "$jexam_diff $out_file $ref_file $jexam_options $stderr";
    // warning($diff_command);
    $diff_retval = execute_command($diff_command);

    if ($clean)
        unlink($out_file);

    return array($diff_retval == 0  ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Output");
}


function test_interpret($src_file, $ref_file, $in_file, $rc_file, $out_file)
{
    global $python_exec, $interpret_path, $diff, $clean, $stderr;

    // executing interpret script
    $command = "$python_exec $interpret_path <$src_file --input $in_file >$out_file $stderr";
    $retval = execute_command($command);

    // reading expected return value
    $exp_retval = (int) file_get_contents($rc_file);

    // comparing return values
    if ($retval != 0)
        return array($exp_retval == $retval ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Return code");

    // executing diff
    $diff_command = "$diff $out_file $ref_file $stderr";
    $diff_retval = execute_command($diff_command);

    if ($clean)
        unlink($out_file);

    return array($diff_retval == 0  ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Output");
}

function test_both($src_file, $ref_file, $in_file, $rc_file, $out_file)
{
    global $php_exec, $parse_path, $python_exec, $interpret_path, $diff, $clean, $stderr;

    // executing parser and interpret script
    $command = "$php_exec $parse_path <$src_file $stderr | $python_exec $interpret_path --input $in_file >$out_file $stderr";
    $retval = execute_command($command);

    // reading expected return value
    $exp_retval = (int) file_get_contents($rc_file);

    // comparing return values
    if ($retval != 0)
        return array($exp_retval == $retval ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Return code");

    // executing diff
    $diff_command = "$diff $out_file $ref_file $stderr";
    $diff_retval = execute_command($diff_command);

    if ($clean)
        unlink($out_file);

    return array($diff_retval == 0  ? TEST_PASS : TEST_FAIL, $exp_retval, $retval, "Output");
}



function testDirContents($dir, $test_function)
{
    global $passed, $bar, $verbose;

    $files = scandir($dir);
    foreach ($files as $key => $value) {
        $path = $dir . DIRECTORY_SEPARATOR . $value;
        if (!is_dir($path) && preg_match("/\.src$/", $path)) {
            $test_path = preg_replace("/\.src$/", "", $path);

            list($res, $expr_rc, $rc, $err_msg) = execute_test($test_function, $test_path);

            if ($res == TEST_PASS)
                $passed++;
            else
                if ($verbose) fprintf(STDERR, "%s, %s\n", "FAIL", $path);

            print_test($res, $test_path, $expr_rc, $rc, $err_msg);
            $bar->tick();
        }
        if (is_dir($path) && $value != "." && $value != ".." && $GLOBALS["recursive"])
            testDirContents($path, $test_function);
    }
}

function gen_test_html($name, $expr_rc, $rc, $err_msg)
{
    $name = basename($name);
    return "<tr><td>$name</td><td>$expr_rc</td><td>$rc</td><td>$err_msg</td></tr>
    ";
}

function print_test($res, $name, $expr_rc, $rc, $err_msg)
{
    global $failed_tests, $passed_tests;
    if ($res == TEST_PASS) {
        $category = dirname($name);
        if (!array_key_exists($category, $passed_tests))
            $passed_tests[$category] = "";

        $passed_tests[$category] .= gen_test_html($name, $expr_rc, $rc, $err_msg);
    }
    if ($res == TEST_FAIL) {
        $category = dirname($name);
        if (!array_key_exists($category, $failed_tests))
            $failed_tests[$category] = "";

        $failed_tests[$category] .= gen_test_html($name, $expr_rc, $rc, $err_msg);
    }
}

function execute_command($command)
{
    exec("timeout 10s " . $command, $output, $retval);
    return $retval;
}

function gen_category_html($category, $text)
{
    return "<tr><td>
    <details>
        <summary>
            $category
        </summary>
        <table class=\"tab2\">
            <tbody>
                $text
            </tbody>
        </table>
    </details>
</td></tr>";
}

$passed_html = "";
foreach ($passed_tests as $category => $text) {
    $passed_html .= gen_category_html($category, $text);
}

$failed_html = "";
foreach ($failed_tests as $category => $text) {
    $failed_html .= gen_category_html($category, $text);
}


$failed = $all_tests - $passed;


echo <<<CFG
<!doctype html>
<html>

<head>
    <title>IPP Tests</title>
</head>

<style>
    .tab1 {
        border-collapse: collapse;
    }

    .tab1 tr {
        border-bottom: 3pt solid black;
    }

    .tab2 {
        border-collapse: collapse;
    }

    .tab2 tr {
        margin-left: 20px;
        border: 1pt solid black;
    }

    .blue {
        color: blue;
    }

    .green {
        color: green;
    }

    .red {
        color: red;
    }
</style>

<body>
    <h1>IPP tests</h1>
    <h2><a class="green">Passed</a>: $passed</h2>
    <h2><a class="red">Failed</a>: $failed</h2>
    <h2>Total: $all_tests</h2>
        <table class="tab1">
            <thead>
                <tr>
                    <th class="red">FAILED</th>
                </tr>
            </thead>
            <tbody>
                $failed_html
            </tbody>
        </table>
    <details>
        <summary class="green">
           <th class="red"> PASSED </th>
        </summary>
        <table class="tab1">
            <thead>
                <tr>
                    <th class="blue">Nazov testu</th>
                </tr>
            </thead>
            <tbody>
                $passed_html
            </tbody>
        </table>
    </details>
</body>

</html>

CFG;



exit(0);
?>