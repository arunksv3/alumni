<?php if (theme_get_setting('scrolltop_display')): ?>
    <div id="toTop"><span class="glyphicon glyphicon-chevron-up"></span></div>
<?php endif; ?>

<?php 
$path = current_path();
global $base_url;
if(strpos($path,'viewjob')==false && strpos($path,'viewresume')==false && strpos($path,'interviewrequest')==false && strpos($path,'viewrequest') ==false && strpos($path,'sendresumerequest')==false && strpos($path,'directshortlist')==false && strpos($path,'latestresume')==false && strpos($path,'viewreguser')==false && strpos($path,'viewevent')==false && strpos($path,'viewnews')==false)
{
?>
<!-- header -->
<header id="header" role="banner" class="clearfix">
    <div class="container">

        <!-- #header-inside -->
        <div id="header-inside" class="clearfix">
            <div class="row">
                <div class="col-md-8">
                    <?php if ($logo): ?>
                        <div id="logo">
                            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /> </a>
                        </div>
                    <?php endif; ?>

                    <?php if ($site_name): ?>
                        <div id="site-name">
                            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
                        </div>
                    <?php endif; ?>

                    <?php if ($site_slogan): ?>
                        <div id="site-slogan">
                            <?php print $site_slogan; ?>
                        </div>
                    <?php endif; ?>

                    <?php if ($page['header']) : ?>
                        <?php print render($page['header']); ?>
                    <?php endif; ?>
                </div>

                <div class="col-md-4">                    

                    <?php if ($primary_links) :
                        if ($user->uid != 1) :
                            ?>
                            <div class="login-sec">
                                <div class="login-le"></div>
                                <div class="login">
                                    <?php print $primary_links; ?>
                                </div>
                                <div class="login-ri"></div>
                            </div>
                        <?php endif;
                    endif; ?>

                    <?php
                        $menu = menu_navigation_links('menu-header-right-menu');
                        $menu = array_slice($menu, 0, 3);
                        print theme('links__menu_header_right_menu', array('links' => $menu, 'attributes' => array('class' => array('top-nav'))));
                    ?>                
                </div>
            </div>
        </div>
        <!-- EOF: #header-inside -->

    </div>
</header>
<!-- EOF: #header --> 

<!-- #main-navigation --> 
<div id="main-navigation" class="clearfix">
    <div class="container">

        <!-- #main-navigation-inside -->
        <div id="main-navigation-inside" class="clearfix">
            <div class="row">
                <div class="col-md-12">
                    <nav role="navigation">
                        <?php
                            if ($page['navigation']) :
                                print drupal_render($page['navigation']);
                            else :
                                print $theme_menu;
                            endif;
                        ?>
                    </nav>
                </div>
            </div>
        </div>
        <!-- EOF: #main-navigation-inside -->

    </div>
</div>
<!-- EOF: #main-navigation -->

<?php
if (drupal_is_front_page()) {
    if ($page['preface_mainpage_slider_block']) :
        ?>
        <!-- #banner -->
        <div id="banner" class="clearfix">
            <div class="container">

                <!-- #banner-inside -->
                <div id="banner-inside" class="clearfix">
                    <div class="row">
                        <div class="col-md-3">
                            <div class="search-graduate-jobs">                                
                                <?php if ($page['preface_sidebar_left_block']) : ?>                                   
                                        <!-- #header-top-left -->
                                        <div id="header-top-left" class="clearfix">
                                            <?php print render($page['preface_sidebar_left_block']); ?>
                                        </div>
                                        <!-- EOF:#header-top-left -->
                                <?php endif; ?>
                            </div>
                        </div>
                        <div class="col-md-9">
                            <div id="slideshow" class="carousel slide carousel-fade">
                                <!-- Indicators -->
                                <ol class="carousel-indicators">
                                    <?php 
                                        $type = "banner"; // machine name for Basic Page
                                        $nodes = node_load_by_type($type, '5');
                                        $i = 0;
                                        foreach ($nodes as $node):
                                            ?><li data-target="#slideshow" data-slide-to="0" class=<?php if ($i == 0) print 'active'; ?>></li><?php
                                            $i++;
                                        endforeach;
                                    ?>                                                                        
                                </ol>

                                <!-- Wrapper for slides -->
                                <div class="carousel-inner">
                                    <?php  
                                    $i = 0;
                                    foreach ($nodes as $node):
                                        ?>
                                        <!-- slider-item -->
                                        <div class="item <?php if ($i == 0) print 'active'; ?>">
                                            <a href="<?php print $node->field_link['und']['0']['value']; ?>"><img src="<?php print file_create_url($node->field_banner['und'][0]['uri']); ?>" class="img-responsive" alt="" /></a>
                                            <div class="carousel-caption">
                                                <h4><?php print $node->title ?></h4>
                                                <p><?php print $node->body["und"]["0"]["value"]; ?></p>
                                            </div>
                                        </div>

                                        <?php
                                        $i++;
                                    endforeach;
                                    ?>
                                </div>

                                <!-- Controls -->
                                <a class="left carousel-control" href="#slideshow" data-slide="prev">
                                    <span class="icon-prev"></span>
                                </a>
                                <a class="right carousel-control" href="#slideshow" data-slide="next">
                                    <span class="icon-next"></span>
                                </a>
                            </div>
                            <script type="text/javascript" charset="utf-8">
                                jQuery(document).ready(function($) {
                                    $(window).load(function() {
                                        $("#slideshow").fadeIn("slow");
                                        $(".carousel").carousel({
                                            interval: '5000',
                                            wrap: true
                                        });
                                        $(".carousel").bind("slide", function(e) {
                                            setTimeout(function () {
                                                var next_h = $(e.relatedTarget).outerHeight();
                                                $(".carousel").css("height", next_h);
                                            }, 10);
                                        });
                                    });
                                });
                            </script>

                        </div>

                    </div>
                </div>
                <!-- EOF: #banner-inside -->        

            </div>
        </div>
        <!-- EOF:#banner -->
    <?php endif; ?>

    <!-- #page -->
    <div id="page" class="clearfix">
        <!-- #main-content -->
        <div id="main-content">
            <div class="container">
                <div class="row m-b-25">                
                    <aside class="col-md-3">  
                        <?php if ($page['sidebar_left_upper_block']) : ?>                                   
                        <!-- #sidebar-left-upper -->                        
                            <?php print render($page['sidebar_left_upper_block']); ?>                    
                        <!-- EOF:#sidebar-left-upper -->
                	<?php endif; ?>
                    </aside>

                    <section class="col-md-6">
                        <!-- #main -->
                        <div id="main" class="clearfix">
                            <div class="home-cont"><h1>Welcome to Career Portal </h1>
                                <h2>IIUM Career Portal - the portal to all things CAREER</h2>
                                <p>A powerful online resource, IIUM Career portal supports your needs for everything related to career at the IIUM. Services, tools, and career resources are only the beginning of how IIUM Career Portal can benefit you in an effort to contribute to your success! Activate your profile today!</p>
                                <p>This is a web-based system designed to help IIUM students to:</p>
                                <ul>
                                    <li>Search and apply for on-and off-campus jobs</li>
                                    <li>View, apply, and sign up for on-campus interviews</li>
                                    <li>Manage career search</li>
                                    <li>Get information about events and workshops</li>
                                    <li>Post resume for employers to see</li>
                                </ul>
                                <p>Log in with username and password same as IIUM email account. </p>   
                                <p>If you have any questions or need help please call us at 
                                    <a href="#">(603) 6196-6303/6313</a> or email <a href="#">css@iium.edu.my.</a></p>
                                <p><span>Note:</span> You'll need to complete your profile and upload a resume to view and apply for jobs in IIUM Career Portal.</p></div>
                        </div>
                        <!-- EOF:#main -->
                    </section>

                    <aside class="col-md-3">
                        <?php if ($page['sidebar_right_upper_block']) : ?>                                   
                        <!-- #sidebar-right-upper-->                        
                            <?php print render($page['sidebar_right_upper_block']); ?>                    
                        <!-- EOF:#sidebar-right-upper -->
                	<?php endif; ?>
                    </aside>
                </div>
                <div class="row"> 
                    <div class="col-md-3">  
                         <?php if ($page['sidebar_left_lower_block']) : ?>                                   
                        <!-- #sidebar-right-upper-->                        
                            <?php print render($page['sidebar_left_lower_block']); ?>                    
                        <!-- EOF:#sidebar-right-upper -->
                	<?php endif; ?>
                    </div>

                    <div class="col-md-6">  
                  	 <?php if ($page['postscript_below_mainpage_content_block']) : ?>                                   
                        <!-- #Most Popular -->                        
                            <?php print render($page['postscript_below_mainpage_content_block']); ?>                    
        		<!-- EOF:#Most Popular-->
                	<?php endif; ?>               
                    </div>

                    <div class="col-md-3">  
                         <?php if ($page['sidebar_right_lower_block']) : ?>                                   
                        <!-- #sidebar-right-upper-->                        
                            <?php print render($page['sidebar_right_lower_block']); ?>                    
                        <!-- EOF:#sidebar-right-upper -->
                	<?php endif; ?>                       
                    </div>

                </div>

            </div>
        </div>
        <!-- EOF:#main-content -->

    </div>
    <!-- EOF:#page -->
<?php
}else {
    ?>
    <div class="inner-title">        
        <div class="container">
            <h1><?php  print $title; ?></h1>
        </div>
    </div>
    <?php 
        if((arg(0)=='employers' && arg(1)!="") || (arg(2)=="jobs")){
            $image = "";
            $key = variable_get('IIUM_Encryption_Key', NULL);
            
            if(arg(2)!="jobs"){
                $jid = "";                
                $q = arg(2);
            }else{
                $jid = (int)decrypt(arg(3),$key);
                $q = arg(1);
            }
             
            $uid = (int)decrypt($q,$key); 
            $employer = employerdetails($uid);  
            $employer_title = remove_words($employer['company_organization']); 
            $jobinfo = jobinfo($jid);
            
            //logo variables
            $logo_vars = array(
                'path' => $employer['company_logo'], 
                'alt' => $employer['company_logo_name'],
                'title' => $employer['company_logo_name'],                       
            );
            
            /* Title */
            if(arg(2)!="jobs"){
                $job_id = "";
                $title = "";
                $image = '<div class="emloyer-logo">'.theme('image', $logo_vars).'</div>
                    <h1>'.$employer['company_organization'].'</h1>';
            }else{
                $title = '<h2>'.$jobinfo['job_title'].'</h2> ';
                $image = '<div class="job-post-logo">'.theme('image', $logo_vars).'
                    <h3>'.l($employer['company_organization'],'employers/'.$employer_title.'/'.encrypt($uid,$key)).'</h3>
                </div>';
                $job_id = '<div class="job-post-id"><label>Job ID</label><span>'.$jobinfo['jobid'].'</span></div>';
            }
            
            
            echo '<div class="subtitle">
                    <div class="container">
                        <div class="employer-header">
                            '.$title.'
                            '.$image.'                       
                            '.$job_id.'   
                        </div>
                    </div>
                </div>';
        }
    ?>
    <div id="main-content">
        <div class="container">

             <!-- #messages-console -->
             <?php if ($messages): ?>
                <div id="messages-console" class="clearfix">
                    <div class="row">
                        <div class="col-md-12">
                             <?php print $messages; ?>
                        </div>
                    </div>
                </div>
             <?php endif; ?>
             <!-- EOF: #messages-console -->

            <div class="row">
                <?php
                if(arg(0) == 'employers'){
                    if(arg(1)==""){
                        $nid = 5;
                        $node = node_load($nid);
                    }
                }else if(arg(1)=="register"){
                    $nid = 42;
                    $node = node_load($nid);
                }else if(arg(1)=="tipsandadvice"){
                    $nid = 45;
                    $node = node_load($nid);
                }else if(arg(0)=='node' && arg(1)==47){
			$node = arg(1);		
		}
                
                if (isset($node)) {
                    if ($node->nid == 40)
                        $page_col = 12;
                    else
                        $page_col = 9;
                }else {
                    $page_col = 12;
                }
                
                if(arg(0) == "faq-page" || arg(0) == "careers" || arg(0) =='tipsandadvice' || arg(0) =='newsandevents' || arg(0) =='eventslist' || arg(0) =='newslist' || arg(0) =='event' || arg(0) =='news' || (arg(0) =='node' && arg(1)==47) || arg(0)=='sitemap' || arg(0) == "psychometrics"){
                    $page_col = 9;
                } 
                    
                $class = "";
                
                ?>

		<?php /* Side bar Menus after login  */ ?>
                <div class="col-md-<?php echo $page_col; ?> inner-cont">
                    <aside class="col-md-3">  
			<?php if ($page['employer_sidebar_menu']) : 
                                $class = "col-md-9 title";  ?>                                   
	                	<!-- #employer_sidebar_menu-->
	                            <?php print render($page['employer_sidebar_menu']); ?>
	                        <!-- EOF:#employer_sidebar_menu -->
	                <?php endif; ?> 
			<?php if ($page['student_sidebar_menu']) : 
                                $class = "col-md-9 title";  ?>                                   
	                	<!-- #student_sidebar_menu-->
	                            <?php print render($page['student_sidebar_menu']); ?>
	                        <!-- EOF:#student_sidebar_menu -->
	                 <?php endif; ?> 
			 <?php if ($page['tips_sidebar_menu']) : 
                                $class = "col-md-9 title"; ?>                                   
	                	<!-- #tips_sidebar_menu-->
	                            <?php print render($page['tips_sidebar_menu']); ?>
	                        <!-- EOF:#tips_sidebar_menu -->
	                 <?php endif; ?> 
			<?php if ($page['events_sidebar_menu']) : 
                                $class = "col-md-9 title"; ?>                                   
	                	<!-- #events_sidebar_menu-->
	                            <?php print render($page['events_sidebar_menu']); ?>
	                        <!-- EOF:#events_sidebar_menu -->
	                 <?php endif; ?>  
                         <?php if ($page['Reports_sidebar_menu']) : 
                                $class = "col-md-9 title"; ?>                                   
	                	<!-- #events_sidebar_menu-->
	                            <?php print render($page['Reports_sidebar_menu']); ?>
	                        <!-- EOF:#events_sidebar_menu -->
	                 <?php endif; ?>  
                    </aside>
                    	<?php   print "<div class='".$class."'>". render($page['mainpage_content_block']). "</div>";  ?>
                </div>                        
                <aside class="col-md-3">  
                        <?php 
                             // get the menu router item for the current page
                             $router_item = menu_get_item(); 
                        ?>
                         <?php if (!$page['employer_sidebar_menu'] && $router_item['title'] != 'Access denied') : ?>
                            <?php if ($page['inner_page_sidebar_right_upper_block']) : ?>                                   
                                   <!-- #footer-third-block-->
                                       <?php print render($page['inner_page_sidebar_right_upper_block']); ?>
                                   <!-- EOF:#footer-third-block -->
                            <?php endif; ?>  
                            <?php if ($page['inner_page_sidebar_right_lower_block']) : ?>                                   
                                   <!-- #footer-third-block-->
                                       <?php print render($page['inner_page_sidebar_right_lower_block']); ?>
                                   <!-- EOF:#footer-third-block -->
                            <?php endif; ?>  
                        <?php endif; ?> 
                </aside>

            </div>
        </div></div>
<?php } ?>
<?php if ($page['footer_first_block'] || $page['footer_second_block'] || $page['footer_third_block'] || $page['footer_fourth_block']): ?>
    <!-- #footer -->
    <footer class="clearfix">
    <div class="footer-title">
        <div class="container">
            <div class="row">
                <div class="col-md-3">
                    <h1>POPULAR INDUSTRIES</h1>
                </div>
                <div class="col-md-3">
                    <h1><?php
                        $useful_title = menu_load('menu-useful-links');
                        print $useful_title['title'];
                    ?>
                    </h1>
                </div>
                <div class="col-md-6">
                    <h1><?php
                        $social_icon_title = menu_load('menu-social-icon-links');
                        print $social_icon_title['title'];
                    ?>
                    </h1>
                </div>
            </div>
        </div>
    </div>
    <div class="footer-cont">
        <div class="container mb-title">
            <div class="row">
                <div class="col-md-3 f-divider">                     
                   <?php if ($page['footer_first_block']) : ?>                                   
                            <!-- #footer-third-block-->
                            <?php print render($page['footer_first_block']); ?>
                            <!-- EOF:#footer-third-block -->
                     <?php endif; ?>  
                </div>

                <div class="col-md-3 f-divider">
                        <?php if ($page['footer_second_block']) : ?>  
                            <h1 class="mb"><?php
                               $useful_title = menu_load('menu-useful-links');
                               print $useful_title['title'];
                                ?>
                            </h1>                                <!-- #footer-second-block -->
                            <div id="header-top-left" class="clearfix">
                                <?php print render($page['footer_second_block']); ?>
                            </div>
                            <!-- EOF:#footer-second-block -->
                	<?php endif; ?>
                </div>

                <div class="col-md-6">
                    <div class="row">
                        <div class="col-md-4 f-divider">                             
                           <?php if ($page['footer_third_block']) : ?>   
                            <h1 class="mb"><?php
                                $social_icon_title = menu_load('menu-social-icon-links');
                                print $social_icon_title['title'];
                            ?></h1>
                        	<!-- #footer-third-block-->
                                <div id="header-top-left" class="clearfix">
                                    <?php print render($page['footer_third_block']); ?>
                                </div>
                                <!-- EOF:#footer-third-block -->
                         <?php endif; ?>  
                        </div>
                        <div class="col-md-8">
                        <?php if ($page['footer_fourth_block']) : ?>                                   
                                <!-- #footer-four-block-->
                                <div id="header-top-left" class="clearfix">
                                    <?php print render($page['footer_fourth_block']); ?>
                                </div>
                                <!-- EOF:#footer-four-block -->
                        <?php endif; ?>                            
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="col-md-12 copy-rights">
                    <?php
                        $menu = menu_navigation_links('menu-terms-privacy-links');
                        print theme('links__menu_terms_privacy_links', array('links' => $menu, 'attributes' => array('class' => array('privacy-links'))));
                        $social_terms_privacy = menu_load('menu-terms-privacy-links');
                        print '<p>' . $social_terms_privacy['description'] . '</p>';
                    ?>                    
                </div>
            </div>
        </div>
    </div>
</footer>
 <!-- #eof footer -->
<?php endif; ?>

<?php }else{
 if($messages)
    print $messages; 
    echo render($page['mainpage_content_block']);
} ?>
