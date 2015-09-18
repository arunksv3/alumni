<?php
/**
 * Function for handling ajax request
 */
function summary_list_ajax($form, $form_state) {
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper_jobsummary']['wrapper'];
}
function summary_list_state_ajax($form, $form_state)
{
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper_jobsummary']['wrapper']['job_country_prefered']['wrapper'];   
}
function summary_list_subparrent_ajax($form, $form_state)
{
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper'];   
}
function summary_list_child_ajax($form, $form_state)
{
   // Return the dropdown list including the wrapper
  return $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper']['thirdspecialization']['wrapper'];    
}
/**
 * Student Summary Report Function.
 */
function summary_reports_submit($form, &$form_state) {
 $form_state['rebuild'] = TRUE;
}
function summary_reports($form, &$form_state) { 
     $display="";
     drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
     drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#to").datepicker("option","minDate", selected)
        }
        });
        $("#to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#from").datepicker("option","maxDate", selected)
        }
        });  
        });', 'inline');
    $form = array();
    $form['filter']['wrapper_jobsummary'] = array(
     '#prefix' => '<div class="summary-report">',
     '#suffix' => '</div>',
    );
    $form['filter']['wrapper_jobsummary']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date Range'),
    );
    $form['filter']['wrapper_jobsummary']['daterange']['from'] = array(
    '#type' => 'textfield', 
    '#id' => 'from', 
    '#title' => 'From',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper_jobsummary']['daterange']['to'] = array(
    '#type' => 'textfield', 
    '#id' => 'to', 
    '#title' => 'To',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper_jobsummary']['summary_report_radios'] = array(
    '#type' => 'radios',
    '#title' => t('<h2>Pick any one of the below to get the summary report</h2>'),
    '#options' => array(1=>'No.of jobs listed',2=>'Job Preferences (in ranking order)'),
    );
    $form['filter']['wrapper_jobsummary']['summary_industry'] = array(
    '#type' => 'select',
    '#options' =>array(0=>'Select',1=>'Nature of business',2=>'Salary range',3=>'Job type',4=>'Location',5=>'Specialization'),
    '#ajax' => array(
       'event'=>'change',
       'callback' => 'summary_list_ajax',
       'wrapper' => 'subparrent-list-wrapper',
     ),
    );
     // Wrapper for sub parent dropdown list
    $form['filter']['wrapper_jobsummary']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper">',
     '#suffix' => '</div>',
    );
     if(isset($form_state['values']['summary_industry'])&& $form_state['values']['summary_industry']!="") {
      if($form_state['values']['summary_industry']==1)
      {
        $form['filter']['wrapper_jobsummary']['wrapper']['industry'] = array(
         '#type' => 'select',
         '#options' =>array(''=>'Select Nature of business')+nature_bussiness_arr(),
        );       
      }
      else if($form_state['values']['summary_industry']==2){
        $form['filter']['wrapper']['salary'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Monthly Salary (MYR) <span>- Enter salary range you are looking for in digits, Ex: 5000 </span>'), 
        ); 

        $form['filter']['wrapper_jobsummary']['wrapper']['salary']['min_salary'] = array(             
            '#type' => 'textfield',
            '#title' => t('Min'),       
        );

        $form['filter']['wrapper_jobsummary']['wrapper']['salary']['max_salary'] = array(             
            '#type' => 'textfield',
            '#title' => t('Max'),      
        );
      }
      else if($form_state['values']['summary_industry']==3)
      {
         $form['filter']['wrapper_jobsummary']['wrapper']['jobtype'] = array(
         '#type' => 'select',
         '#options' =>array(''=>'select type')+jobtype(),
        );   
      }
     else if($form_state['values']['summary_industry']==4)
      {
        $form['filter']['wrapper_jobsummary']['wrapper']['job_country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'summary_list_state_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
       );
        // Wrapper for state dropdown list
      $form['filter']['wrapper_jobsummary']['wrapper']['job_country_prefered']['wrapper'] = array(
          '#prefix' => '<div id="state-list-wrapper">',
          '#suffix' => '</div>',
       );
       if ((isset($form_state['values']['job_country']) && $form_state['values']['job_country'] != ""))
       {
           $cid = $form_state['values']['job_country'];
           $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['filter']['wrapper_jobsummary']['wrapper']['job_country_prefered']['wrapper']['job_state'] = array(
                '#type' => 'select',
                '#title' => t('State'),
                '#options' => $state_list_arr,
            );
        }
       }
      }
     else if($form_state['values']['summary_industry']==5)
      {
         $category_parrents = db_select('specializations_p')
                           ->fields('specializations_p') 
                           ->execute()
                           ->fetchAll(); 
         $display_category_parrents['']= 'All Specializations';   
          foreach($category_parrents as $category_parrent_name): 
            $display_category_parrents[$category_parrent_name->sid]= $category_parrent_name->name;   
          endforeach;        
         $form['filter']['wrapper_jobsummary']['wrapper']['job_specializations'] = array(
         '#type' => 'select',
         '#title' => t('Specialization'),
         '#options' => $display_category_parrents,
         '#ajax' => array(
         'event'=>'change',
         'callback' => 'summary_list_subparrent_ajax',
         'wrapper' => 'summary-list-wrapper',
          ),
        );
        // Wrapper for sub parent dropdown list
        $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper'] = array(
         '#prefix' => '<div id="summary-list-wrapper">',
         '#suffix' => '</div>',
        ); 
        if (isset($form_state['values']['job_specializations'])&& $form_state['values']['job_specializations']!="") {
         $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$form_state['values']['job_specializations'], '=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'All';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
        $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper']['sub_parent'] = array(
         '#type' => 'select',
         '#options' => $display_category_subparrents,
         '#ajax' => array(
          'event'=>'change',
          'callback' => 'summary_list_child_ajax',
          'wrapper' => 'child-list-wrapper',
         ),
        );         
       }
        // Wrapper for child dropdown list
        $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper']['thirdspecialization']['wrapper'] = array(
         '#prefix' => '<div id="child-list-wrapper">',
         '#suffix' => '</div>',
        );
        if (isset($form_state['values']['sub_parent'])&& $form_state['values']['sub_parent']!="") {
            $category_childs = db_select('specializations_c')
                               ->fields('specializations_c') 
                               ->condition('cid',$form_state['values']['sub_parent'], '=')   
                               ->execute()
                               ->fetchAll(); 
            $display_category_childs['']= 'All';   
            foreach($category_childs as $category_child_name): 
              $display_category_childs[$category_child_name->sid]= $category_child_name->name;   
            endforeach;
             // Province dropdown list
         $form['filter']['wrapper_jobsummary']['wrapper']['second_specialization']['wrapper']['thirdspecialization']['wrapper']['childs'] = array(
          '#type' => 'select',
          '#options' => $display_category_childs,
         );         
        }
     }
    }
    $form['filter']['wrapper_jobsummary']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('summary_reports_submit'),
    );
    if (isset($form_state['values']['summary_report_radios'])&& $form_state['values']['summary_report_radios']!="" && $form_state['values']['op']!="") {
      $display_first='<span class="result">Result:</span><div class="print-down"><a href="javascript:window.print();">Print Report</a> | <a href="?pdf=pdf">Download as PDF</a></div>';
      if($form_state['values']['summary_report_radios']==1)
      {
         $results = db_select('employer_jobs','e');
         $results->fields('e');

         if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
          //$fromdate = date('Y-m-d',strtotime($form_state['values']['from']));
          $results->condition('e.date_time',$fromdate,'>=');
         }
         if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
           $to_input = explode('/',$form_state['values']['to']);
           $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
          //$todate = date('Y-m-d',strtotime($form_state['values']['to']));
           $results->condition('e.date_time',$todate,'<=');
         }

         $results_count=$results->execute();
         $count = $results_count->rowCount();
         if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
          $display="<div class='reslt-cont'>No.of job listed from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
         else 
          $display="<div class='reslt-cont'>No.of jobs listed till date is: $count</div>";
       } 
       else if($form_state['values']['summary_report_radios']==2)
       {
         if($form_state['values']['summary_industry']==1)
         {
           $results = db_select('employer_registration','r');
           //$results->innerjoin('employer_registration', 'r', 'r.uid = e.uid');
           $results->fields('r',array('uid'));
           
          if($form_state['values']['industry']!="") 
          {
            $db_or = db_or();
            $db_or->condition('r.nature_bussiness',db_like($form_state['values']['industry']).'|%','LIKE');
            $db_or->condition('r.nature_bussiness',$form_state['values']['industry'],'=');
            $db_or->condition('r.nature_bussiness','%|'.db_like($form_state['values']['industry']).'|%','LIKE');
            $db_or->condition('r.nature_bussiness','%|'.db_like($form_state['values']['industry']),'LIKE');
            $results->condition($db_or);
          } 
          
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
           {
             $from_input = explode('/',$form_state['values']['from']);
             $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
             $results->condition('r.created_date',$fromdate,'>=');
           }
          if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
          {
            $to_input = explode('/',$form_state['values']['to']);
            $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
            $results->condition('r.created_date',$todate,'<=');
          }

          $results_count=$results->execute();
          $count = $results_count->rowCount();
          $nature_bussiness_arr=nature_bussiness_arr();
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
           $display="<div class='reslt-cont'> Job Preferences (Nature of business->".$nature_bussiness_arr[$form_state['values']['industry']].")  from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
          else 
           $display="<div class='reslt-cont'>Job Preferences (Nature of business->".$nature_bussiness_arr[$form_state['values']['industry']].") till date is: $count</div>";
           
         }
         else if($form_state['values']['summary_industry']==2)
         {
           $results = db_select('employer_jobs','e');
           $results->fields('e',array('jobid'));           
          
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
           {
             $from_input = explode('/',$form_state['values']['from']);
             $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
             $results->condition('e.date_time',$fromdate,'>=');
           }
          if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
           {
            $to_input = explode('/',$form_state['values']['to']);
            $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
            $results->condition('e.date_time',$todate,'<=');
           }
          if($form_state['values']['min_salary']!="" && $form_state['values']['max_salary']!="") 
           {
             $min_salary = $form_state['values']['min_salary'];
             $max_salary = $form_state['values']['max_salary'];  
             $results->condition('e.salary', array($min_salary, $max_salary), 'BETWEEN');
           } 
            $results_count=$results->execute();
            $count = $results_count->rowCount();
            
            if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
             $display="<div class='reslt-cont'> Job Preferences (Salary range-> Min ".$min_salary."-Max ".$max_salary.")  from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
            else 
             $display="<div class='reslt-cont'>Job Preferences (Salary range->Min ".$min_salary."-Max ".$max_salary.") till date is: $count</div>"; 
           
           }
         else if($form_state['values']['summary_industry']==3)
         {
           $results = db_select('employer_jobs','e');
           $results->fields('e',array('jobid'));           
          
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
           {
            $from_input = explode('/',$form_state['values']['from']);
            $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
            $results->condition('e.date_time',$fromdate,'>=');
           }
          if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
           {
            $to_input = explode('/',$form_state['values']['to']);
            $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
            $results->condition('e.date_time',$todate,'<=');
           }
          if($form_state['values']['jobtype']!="") 
           {
             $db_or = db_or();
             $db_or->condition('e.jobs_type',db_like($form_state['values']['jobtype']).',%','LIKE');
             $db_or->condition('e.jobs_type',$form_state['values']['jobtype'],'=');
             $db_or->condition('e.jobs_type','%,' .db_like($form_state['values']['jobtype']).',%','LIKE');
             $db_or->condition('e.jobs_type','%,' .db_like($form_state['values']['jobtype']),'LIKE');
             $results->condition($db_or);
           } 
            $results_count=$results->execute();
            $count = $results_count->rowCount();
            $jobtype=jobtype(); 
            
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
           $display="<div class='reslt-cont'> Job Preferences (Job type->".$jobtype[$form_state['values']['jobtype']].")  from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
          else 
           $display="<div class='reslt-cont'>Job Preferences (Job type->".$jobtype[$form_state['values']['jobtype']].") till date is: $count</div>";
           
           }  
         else if($form_state['values']['summary_industry']==4)
         {
           $results = db_select('employer_jobs','e');
           $results->fields('e',array('jobid'));           
          
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
           {
            $from_input = explode('/',$form_state['values']['from']);
            $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
            $results->condition('e.date_time',$fromdate,'>=');
           }
           if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
           {
            $to_input = explode('/',$form_state['values']['to']);
            $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
            $results->condition('e.date_time',$todate,'<=');
           }
           if($form_state['values']['job_country']!="") 
           {
             $results->condition('e.job_country', trim($form_state['values']['job_country']), '=');
           } 
           if(isset($form_state['values']['job_state']) && $form_state['values']['job_state']!=""){ 
             $results->condition('e.job_state', $form_state['values']['job_state'], '=');
            }
            $results_count=$results->execute();
            $count = $results_count->rowCount();
            $countries_list=countries_list();
            
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
           $display="<div class='reslt-cont'> Job Preferences (Location->".$countries_list[$form_state['values']['job_country']].")  from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
          else 
           $display="<div class='reslt-cont'>Job Preferences (Location->".$countries_list[$form_state['values']['job_country']].") till date is: $count</div>";
           
         } 
         else if($form_state['values']['summary_industry']==5)
         {
           $results = db_select('employer_jobs','e');
           $results->fields('e',array('jobid'));           
          
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
           {
            $from_input = explode('/',$form_state['values']['from']);
            $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
            $results->condition('e.date_time',$fromdate,'>=');
           }
           if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
           {
            $to_input = explode('/',$form_state['values']['to']);
            $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");
            $results->condition('e.date_time',$todate,'<=');
           }
           if (isset($form_state['values']['job_specializations']) && $form_state['values']['job_specializations']!="") {
             $specializations = $form_state['values']['job_specializations'];
             $db_or = db_or();
             $db_or->condition('e.specializations_parrent', db_like($specializations).',%','LIKE');
             $db_or->condition('e.specializations_parrent', trim($specializations), '=');
             $db_or->condition('e.specializations_parrent','%,' .db_like($specializations).',%','LIKE');
             $db_or->condition('e.specializations_parrent','%,' .db_like($specializations),'LIKE');
             $results->condition($db_or);
             $specialization_disp=getSpecializationParentName($specializations);
            }
           if (isset($form_state['values']['sub_parent']) && $form_state['values']['sub_parent']!="") {
             $sub_parent = $form_state['values']['sub_parent'];
             $db_or = db_or();
             $db_or->condition('e.specializations_subparrent', db_like($sub_parent).',%','LIKE');
             $db_or->condition('e.specializations_subparrent', trim($sub_parent),'=');
             $db_or->condition('e.specializations_subparrent','%,' .db_like($sub_parent).',%','LIKE');
             $db_or->condition('e.specializations_subparrent','%,' .db_like($sub_parent),'LIKE');
             $results->condition($db_or);
             $specialization_disp.='->'.getSpecializationSubparent($sub_parent);
            } 
           if(isset($form_state['values']['childs']) && $form_state['values']['childs']!=""){
             $childs = $form_state['values']['childs'];
             $db_or = db_or();
             $db_or->condition('e.specializations', db_like(trim($childs)).',%','LIKE');
             $db_or->condition('e.specializations', trim($childs), '=');
             $db_or->condition('e.specializations','%,' .db_like(trim($childs)).',%','LIKE');
             $db_or->condition('e.specializations','%,' .db_like(trim($childs)),'LIKE');
             $results->condition($db_or);   
             $specialization_disp.='->'.getSpecializationChild($childs);
            }      
           
            $results_count=$results->execute();
            $count = $results_count->rowCount();
            
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
           $display="<div class='reslt-cont'> Job Preferences (Specialization->".$specialization_disp.")  from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
          else 
           $display="<div class='reslt-cont'>Job Preferences (Specialization->".$specialization_disp.") till date is: $count</div>";
           
         } 
    
        }    
      }
    if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        </head>
        <body>
        <div style="text-align:center">'. $_SESSION['display'].'</div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("students_summary.pdf");
    }
    $_SESSION['display'] = $display;
    $form['filter']['table'] = array(
      '#type' => 'fieldset', 
      '#title' =>$display_first.$display,

    );
    return $form;
}