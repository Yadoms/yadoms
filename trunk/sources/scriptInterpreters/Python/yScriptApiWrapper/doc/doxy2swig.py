


<!DOCTYPE html>
<html lang="en" class=" is-copy-enabled">
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# object: http://ogp.me/ns/object# article: http://ogp.me/ns/article# profile: http://ogp.me/ns/profile#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Language" content="en">
    <meta name="viewport" content="width=1020">
    
    
    <title>doxy2swig/doxy2swig.py at master · m7thon/doxy2swig · GitHub</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub">
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub">
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png">
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png">
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png">
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png">
    <meta property="fb:app_id" content="1401488693436528">

      <meta content="@github" name="twitter:site" /><meta content="summary" name="twitter:card" /><meta content="m7thon/doxy2swig" name="twitter:title" /><meta content="doxy2swig - Doxygen XML to SWIG docstring converter." name="twitter:description" /><meta content="https://avatars0.githubusercontent.com/u/5876537?v=3&amp;s=400" name="twitter:image:src" />
      <meta content="GitHub" property="og:site_name" /><meta content="object" property="og:type" /><meta content="https://avatars0.githubusercontent.com/u/5876537?v=3&amp;s=400" property="og:image" /><meta content="m7thon/doxy2swig" property="og:title" /><meta content="https://github.com/m7thon/doxy2swig" property="og:url" /><meta content="doxy2swig - Doxygen XML to SWIG docstring converter." property="og:description" />
      <meta name="browser-stats-url" content="https://api.github.com/_private/browser/stats">
    <meta name="browser-errors-url" content="https://api.github.com/_private/browser/errors">
    <link rel="assets" href="https://assets-cdn.github.com/">
    
    <meta name="pjax-timeout" content="1000">
    

    <meta name="msapplication-TileImage" content="/windows-tile.png">
    <meta name="msapplication-TileColor" content="#ffffff">
    <meta name="selected-link" value="repo_source" data-pjax-transient>

    <meta name="google-site-verification" content="KT5gs8h0wvaagLKAVWq8bbeNwnZZK1r1XQysX3xurLU">
        <meta name="google-analytics" content="UA-3769691-2">

    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="collector-cdn.github.com" name="octolytics-script-host" /><meta content="github" name="octolytics-app-id" /><meta content="51FC3EB4:0699:19D22AC:55E6EA6D" name="octolytics-dimension-request_id" />
    
    <meta content="Rails, view, blob#show" data-pjax-transient="true" name="analytics-event" />
    <meta class="js-ga-set" name="dimension1" content="Logged Out">
      <meta class="js-ga-set" name="dimension4" content="Current repo nav">
    <meta name="is-dotcom" content="true">
        <meta name="hostname" content="github.com">
    <meta name="user-login" content="">

      <link rel="icon" sizes="any" mask href="https://assets-cdn.github.com/pinned-octocat.svg">
      <meta name="theme-color" content="#4078c0">
      <link rel="icon" type="image/x-icon" href="https://assets-cdn.github.com/favicon.ico">

    <!-- </textarea> --><!-- '"` --><meta content="authenticity_token" name="csrf-param" />
<meta content="ON20ertlS8lLjXhj5IJCUrZX1Di+r/ffY77ztYNtUZKOru+4ZlFQHfO+P1rhhPvmzefORCjKrjcqpPwQSs254A==" name="csrf-token" />
    

    <link crossorigin="anonymous" href="https://assets-cdn.github.com/assets/github-2adcab8ffefe91050e3f2b4f6c61225e182734ec89373323b874e0518954aaaa.css" media="all" rel="stylesheet" />
    <link crossorigin="anonymous" href="https://assets-cdn.github.com/assets/github2-0f9ba210819ce56d9f786431efa5742ecacb9ea2d491b3b2a1c191626fd447e7.css" media="all" rel="stylesheet" />
    
    


    <meta http-equiv="x-pjax-version" content="e89d6be9167a144c8bcaa49a7ef7eaa2">

      
  <meta name="description" content="doxy2swig - Doxygen XML to SWIG docstring converter.">
  <meta name="go-import" content="github.com/m7thon/doxy2swig git https://github.com/m7thon/doxy2swig.git">

  <meta content="5876537" name="octolytics-dimension-user_id" /><meta content="m7thon" name="octolytics-dimension-user_login" /><meta content="38275267" name="octolytics-dimension-repository_id" /><meta content="m7thon/doxy2swig" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="false" name="octolytics-dimension-repository_is_fork" /><meta content="38275267" name="octolytics-dimension-repository_network_root_id" /><meta content="m7thon/doxy2swig" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/m7thon/doxy2swig/commits/master.atom" rel="alternate" title="Recent Commits to doxy2swig:master" type="application/atom+xml">

  </head>


  <body class="logged_out  env-production windows vis-public page-blob">
    <a href="#start-of-content" tabindex="1" class="accessibility-aid js-skip-to-content">Skip to content</a>
    <div class="wrapper">
      
      
      



        
        <div class="header header-logged-out" role="banner">
  <div class="container clearfix">

    <a class="header-logo-wordmark" href="https://github.com/" data-ga-click="(Logged out) Header, go to homepage, icon:logo-wordmark">
      <span class="mega-octicon octicon-logo-github"></span>
    </a>

    <div class="header-actions" role="navigation">
        <a class="btn btn-primary" href="/join" data-ga-click="(Logged out) Header, clicked Sign up, text:sign-up">Sign up</a>
      <a class="btn" href="/login?return_to=%2Fm7thon%2Fdoxy2swig%2Fblob%2Fmaster%2Fdoxy2swig.py" data-ga-click="(Logged out) Header, clicked Sign in, text:sign-in">Sign in</a>
    </div>

    <div class="site-search repo-scope js-site-search" role="search">
      <!-- </textarea> --><!-- '"` --><form accept-charset="UTF-8" action="/m7thon/doxy2swig/search" class="js-site-search-form" data-global-search-url="/search" data-repo-search-url="/m7thon/doxy2swig/search" method="get"><div style="margin:0;padding:0;display:inline"><input name="utf8" type="hidden" value="&#x2713;" /></div>
  <label class="js-chromeless-input-container form-control">
    <div class="scope-badge">This repository</div>
    <input type="text"
      class="js-site-search-focus js-site-search-field is-clearable chromeless-input"
      data-hotkey="s"
      name="q"
      placeholder="Search"
      aria-label="Search this repository"
      data-global-scope-placeholder="Search GitHub"
      data-repo-scope-placeholder="Search"
      tabindex="1"
      autocapitalize="off">
  </label>
</form>
    </div>

      <ul class="header-nav left" role="navigation">
          <li class="header-nav-item">
            <a class="header-nav-link" href="/explore" data-ga-click="(Logged out) Header, go to explore, text:explore">Explore</a>
          </li>
          <li class="header-nav-item">
            <a class="header-nav-link" href="/features" data-ga-click="(Logged out) Header, go to features, text:features">Features</a>
          </li>
          <li class="header-nav-item">
            <a class="header-nav-link" href="https://enterprise.github.com/" data-ga-click="(Logged out) Header, go to enterprise, text:enterprise">Enterprise</a>
          </li>
          <li class="header-nav-item">
            <a class="header-nav-link" href="/pricing" data-ga-click="(Logged out) Header, go to pricing, text:pricing">Pricing</a>
          </li>
      </ul>

  </div>
</div>



      <div id="start-of-content" class="accessibility-aid"></div>

      <div id="js-flash-container">
</div>


          <div itemscope itemtype="http://schema.org/WebPage">
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">

        <div class="clearfix">
          
<ul class="pagehead-actions">

  <li>
      <a href="/login?return_to=%2Fm7thon%2Fdoxy2swig"
    class="btn btn-sm btn-with-count tooltipped tooltipped-n"
    aria-label="You must be signed in to watch a repository" rel="nofollow">
    <span class="octicon octicon-eye"></span>
    Watch
  </a>
  <a class="social-count" href="/m7thon/doxy2swig/watchers">
    1
  </a>

  </li>

  <li>
      <a href="/login?return_to=%2Fm7thon%2Fdoxy2swig"
    class="btn btn-sm btn-with-count tooltipped tooltipped-n"
    aria-label="You must be signed in to star a repository" rel="nofollow">
    <span class="octicon octicon-star"></span>
    Star
  </a>

    <a class="social-count js-social-count" href="/m7thon/doxy2swig/stargazers">
      0
    </a>

  </li>

    <li>
      <a href="/login?return_to=%2Fm7thon%2Fdoxy2swig"
        class="btn btn-sm btn-with-count tooltipped tooltipped-n"
        aria-label="You must be signed in to fork a repository" rel="nofollow">
        <span class="octicon octicon-repo-forked"></span>
        Fork
      </a>
      <a href="/m7thon/doxy2swig/network" class="social-count">
        0
      </a>
    </li>
</ul>

          <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public ">
  <span class="mega-octicon octicon-repo"></span>
  <span class="author"><a href="/m7thon" class="url fn" itemprop="url" rel="author"><span itemprop="title">m7thon</span></a></span><!--
--><span class="path-divider">/</span><!--
--><strong><a href="/m7thon/doxy2swig" data-pjax="#js-repo-pjax-container">doxy2swig</a></strong>

  <span class="page-context-loader">
    <img alt="" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
  </span>

</h1>

        </div>
      </div>
    </div>

    <div class="container">
      <div class="repository-with-sidebar repo-container new-discussion-timeline ">
        <div class="repository-sidebar clearfix">
          
<nav class="sunken-menu repo-nav js-repo-nav js-sidenav-container-pjax js-octicon-loaders"
     role="navigation"
     data-pjax="#js-repo-pjax-container"
     data-issue-count-url="/m7thon/doxy2swig/issues/counts">
  <ul class="sunken-menu-group">
    <li class="tooltipped tooltipped-w" aria-label="Code">
      <a href="/m7thon/doxy2swig" aria-label="Code" aria-selected="true" class="js-selected-navigation-item selected sunken-menu-item" data-hotkey="g c" data-selected-links="repo_source repo_downloads repo_commits repo_releases repo_tags repo_branches /m7thon/doxy2swig">
        <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
        <img alt="" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>    </li>

      <li class="tooltipped tooltipped-w" aria-label="Issues">
        <a href="/m7thon/doxy2swig/issues" aria-label="Issues" class="js-selected-navigation-item sunken-menu-item" data-hotkey="g i" data-selected-links="repo_issues repo_labels repo_milestones /m7thon/doxy2swig/issues">
          <span class="octicon octicon-issue-opened"></span> <span class="full-word">Issues</span>
          <span class="js-issue-replace-counter"></span>
          <img alt="" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

    <li class="tooltipped tooltipped-w" aria-label="Pull requests">
      <a href="/m7thon/doxy2swig/pulls" aria-label="Pull requests" class="js-selected-navigation-item sunken-menu-item" data-hotkey="g p" data-selected-links="repo_pulls /m7thon/doxy2swig/pulls">
          <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull requests</span>
          <span class="js-pull-replace-counter"></span>
          <img alt="" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>    </li>

  </ul>
  <div class="sunken-menu-separator"></div>
  <ul class="sunken-menu-group">

    <li class="tooltipped tooltipped-w" aria-label="Pulse">
      <a href="/m7thon/doxy2swig/pulse" aria-label="Pulse" class="js-selected-navigation-item sunken-menu-item" data-selected-links="pulse /m7thon/doxy2swig/pulse">
        <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
        <img alt="" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>    </li>

    <li class="tooltipped tooltipped-w" aria-label="Graphs">
      <a href="/m7thon/doxy2swig/graphs" aria-label="Graphs" class="js-selected-navigation-item sunken-menu-item" data-selected-links="repo_graphs repo_contributors /m7thon/doxy2swig/graphs">
        <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
        <img alt="" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>    </li>
  </ul>


</nav>

            <div class="only-with-full-nav">
                
<div class="js-clone-url clone-url open"
  data-protocol-type="http">
  <h3><span class="text-emphasized">HTTPS</span> clone URL</h3>
  <div class="input-group js-zeroclipboard-container">
    <input type="text" class="input-mini input-monospace js-url-field js-zeroclipboard-target"
           value="https://github.com/m7thon/doxy2swig.git" readonly="readonly" aria-label="HTTPS clone URL">
    <span class="input-group-button">
      <button aria-label="Copy to clipboard" class="js-zeroclipboard btn btn-sm zeroclipboard-button tooltipped tooltipped-s" data-copied-hint="Copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </span>
  </div>
</div>

  
<div class="js-clone-url clone-url "
  data-protocol-type="subversion">
  <h3><span class="text-emphasized">Subversion</span> checkout URL</h3>
  <div class="input-group js-zeroclipboard-container">
    <input type="text" class="input-mini input-monospace js-url-field js-zeroclipboard-target"
           value="https://github.com/m7thon/doxy2swig" readonly="readonly" aria-label="Subversion checkout URL">
    <span class="input-group-button">
      <button aria-label="Copy to clipboard" class="js-zeroclipboard btn btn-sm zeroclipboard-button tooltipped tooltipped-s" data-copied-hint="Copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </span>
  </div>
</div>



  <div class="clone-options">You can clone with
    <!-- </textarea> --><!-- '"` --><form accept-charset="UTF-8" action="/users/set_protocol?protocol_selector=http&amp;protocol_type=clone" class="inline-form js-clone-selector-form " data-form-nonce="e7c177ed12f579bd13114246d8d00a5279fa8787" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="utf8" type="hidden" value="&#x2713;" /><input name="authenticity_token" type="hidden" value="3Kg0T3ouux9IbcpyEcecEttfeixqj9iAULBJlZRavEzeNmgwmwusZ7eMs9gYtYPdhda7H0+T12fA4D2taP5qJA==" /></div><button class="btn-link js-clone-selector" data-protocol="http" type="submit">HTTPS</button></form> or <!-- </textarea> --><!-- '"` --><form accept-charset="UTF-8" action="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=clone" class="inline-form js-clone-selector-form " data-form-nonce="e7c177ed12f579bd13114246d8d00a5279fa8787" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="utf8" type="hidden" value="&#x2713;" /><input name="authenticity_token" type="hidden" value="1rgGGO5kuQANU1hpzRMmBRqmj+w3nAiGDDyDI0kCjrxIAeQsoX9dXJHMz+zRc8Sk0l2kCNiSO3e873euBhnWwA==" /></div><button class="btn-link js-clone-selector" data-protocol="subversion" type="submit">Subversion</button></form>.
    <a href="https://help.github.com/articles/which-remote-url-should-i-use" class="help tooltipped tooltipped-n" aria-label="Get help on which URL is right for you.">
      <span class="octicon octicon-question"></span>
    </a>
  </div>
    <a href="https://windows.github.com" class="btn btn-sm sidebar-button" title="Save m7thon/doxy2swig to your computer and use it in GitHub Desktop." aria-label="Save m7thon/doxy2swig to your computer and use it in GitHub Desktop.">
      <span class="octicon octicon-desktop-download"></span>
      Clone in Desktop
    </a>

              <a href="/m7thon/doxy2swig/archive/master.zip"
                 class="btn btn-sm sidebar-button"
                 aria-label="Download the contents of m7thon/doxy2swig as a zip file"
                 title="Download the contents of m7thon/doxy2swig as a zip file"
                 rel="nofollow">
                <span class="octicon octicon-cloud-download"></span>
                Download ZIP
              </a>
            </div>
        </div>
        <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>

          

<a href="/m7thon/doxy2swig/blob/6b447d9d6aed75ff3949a72b252a100b6b89b374/doxy2swig.py" class="hidden js-permalink-shortcut" data-hotkey="y">Permalink</a>

<!-- blob contrib key: blob_contributors:v21:c696d45d52b1722ef443ffc995591792 -->

  <div class="file-navigation js-zeroclipboard-container">
    
<div class="select-menu js-menu-container js-select-menu left">
  <span class="btn btn-sm select-menu-button js-menu-target css-truncate" data-hotkey="w"
    data-ref="master"
    title="master"
    role="button" aria-label="Switch branches or tags" tabindex="0" aria-haspopup="true">
    <i>Branch:</i>
    <span class="js-select-button css-truncate-target">master</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax aria-hidden="true">

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-x js-menu-close" role="button" aria-label="Close"></span>
      </div>

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" aria-label="Filter branches/tags" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" data-filter-placeholder="Filter branches/tags" class="js-select-menu-tab" role="tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" data-filter-placeholder="Find a tag…" class="js-select-menu-tab" role="tab">Tags</a>
            </li>
          </ul>
        </div>
      </div>

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches" role="menu">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <a class="select-menu-item js-navigation-item js-navigation-open selected"
               href="/m7thon/doxy2swig/blob/master/doxy2swig.py"
               data-name="master"
               data-skip-pjax="true"
               rel="nofollow">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <span class="select-menu-item-text css-truncate-target" title="master">
                master
              </span>
            </a>
        </div>

          <div class="select-menu-no-results">Nothing to show</div>
      </div>

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div>

    </div>
  </div>
</div>

    <div class="btn-group right">
      <a href="/m7thon/doxy2swig/find/master"
            class="js-show-file-finder btn btn-sm empty-icon tooltipped tooltipped-nw"
            data-pjax
            data-hotkey="t"
            aria-label="Quickly jump between files">
        <span class="octicon octicon-list-unordered"></span>
      </a>
      <button aria-label="Copy file path to clipboard" class="js-zeroclipboard btn btn-sm zeroclipboard-button tooltipped tooltipped-s" data-copied-hint="Copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </div>

    <div class="breadcrumb js-zeroclipboard-target">
      <span class="repo-root js-repo-root"><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/m7thon/doxy2swig" class="" data-branch="master" data-pjax="true" itemscope="url"><span itemprop="title">doxy2swig</span></a></span></span><span class="separator">/</span><strong class="final-path">doxy2swig.py</strong>
    </div>
  </div>


  <div class="commit file-history-tease">
    <div class="file-history-tease-header">
        <img alt="@m7thon" class="avatar" height="24" src="https://avatars1.githubusercontent.com/u/5876537?v=3&amp;s=48" width="24" />
        <span class="author"><a href="/m7thon" rel="author">m7thon</a></span>
        <time datetime="2015-08-04T23:26:33Z" is="relative-time">Aug 5, 2015</time>
        <div class="commit-title">
            <a href="/m7thon/doxy2swig/commit/6b447d9d6aed75ff3949a72b252a100b6b89b374" class="message" data-pjax="true" title="doxy2swig.py README.md improvement">doxy2swig.py README.md improvement</a>
        </div>
    </div>

    <div class="participation">
      <p class="quickstat">
        <a href="#blob_contributors_box" rel="facebox">
          <strong>1</strong>
           contributor
        </a>
      </p>
      
    </div>
    <div id="blob_contributors_box" style="display:none">
      <h2 class="facebox-header" data-facebox-id="facebox-header">Users who have contributed to this file</h2>
      <ul class="facebox-user-list" data-facebox-id="facebox-description">
          <li class="facebox-user-list-item">
            <img alt="@m7thon" height="24" src="https://avatars1.githubusercontent.com/u/5876537?v=3&amp;s=48" width="24" />
            <a href="/m7thon">m7thon</a>
          </li>
      </ul>
    </div>
  </div>

<div class="file">
  <div class="file-header">
    <div class="file-actions">

      <div class="btn-group">
        <a href="/m7thon/doxy2swig/raw/master/doxy2swig.py" class="btn btn-sm " id="raw-url">Raw</a>
          <a href="/m7thon/doxy2swig/blame/master/doxy2swig.py" class="btn btn-sm js-update-url-with-hash">Blame</a>
        <a href="/m7thon/doxy2swig/commits/master/doxy2swig.py" class="btn btn-sm " rel="nofollow">History</a>
      </div>

        <a class="octicon-btn tooltipped tooltipped-nw"
           href="https://windows.github.com"
           aria-label="Open this file in GitHub Desktop"
           data-ga-click="Repository, open with desktop, type:windows">
            <span class="octicon octicon-device-desktop"></span>
        </a>

          <button type="button" class="octicon-btn disabled tooltipped tooltipped-n" aria-label="You must be signed in to make or propose changes">
            <span class="octicon octicon-pencil"></span>
          </button>

        <button type="button" class="octicon-btn octicon-btn-danger disabled tooltipped tooltipped-n" aria-label="You must be signed in to make or propose changes">
          <span class="octicon octicon-trashcan"></span>
        </button>
    </div>

    <div class="file-info">
        <span class="file-mode" title="File mode">executable file</span>
        <span class="file-info-divider"></span>
        838 lines (754 sloc)
        <span class="file-info-divider"></span>
      34.364 kB
    </div>
  </div>
  

  <div class="blob-wrapper data type-python">
      <table class="highlight tab-size js-file-line-container" data-tab-size="8">
      <tr>
        <td id="L1" class="blob-num js-line-number" data-line-number="1"></td>
        <td id="LC1" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#!/usr/bin/env python</span></td>
      </tr>
      <tr>
        <td id="L2" class="blob-num js-line-number" data-line-number="2"></td>
        <td id="LC2" class="blob-code blob-code-inner js-file-line"><span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>doxy2swig.py [options] index.xml output.i</span></td>
      </tr>
      <tr>
        <td id="L3" class="blob-num js-line-number" data-line-number="3"></td>
        <td id="LC3" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L4" class="blob-num js-line-number" data-line-number="4"></td>
        <td id="LC4" class="blob-code blob-code-inner js-file-line"><span class="pl-s">Doxygen XML to SWIG docstring converter (improved version).</span></td>
      </tr>
      <tr>
        <td id="L5" class="blob-num js-line-number" data-line-number="5"></td>
        <td id="LC5" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L6" class="blob-num js-line-number" data-line-number="6"></td>
        <td id="LC6" class="blob-code blob-code-inner js-file-line"><span class="pl-s">Converts Doxygen generated XML files into a file containing docstrings</span></td>
      </tr>
      <tr>
        <td id="L7" class="blob-num js-line-number" data-line-number="7"></td>
        <td id="LC7" class="blob-code blob-code-inner js-file-line"><span class="pl-s">for use by SWIG.</span></td>
      </tr>
      <tr>
        <td id="L8" class="blob-num js-line-number" data-line-number="8"></td>
        <td id="LC8" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L9" class="blob-num js-line-number" data-line-number="9"></td>
        <td id="LC9" class="blob-code blob-code-inner js-file-line"><span class="pl-s">index.xml is your doxygen generated XML file and output.i is where the</span></td>
      </tr>
      <tr>
        <td id="L10" class="blob-num js-line-number" data-line-number="10"></td>
        <td id="LC10" class="blob-code blob-code-inner js-file-line"><span class="pl-s">output will be written (the file will be clobbered).</span></td>
      </tr>
      <tr>
        <td id="L11" class="blob-num js-line-number" data-line-number="11"></td>
        <td id="LC11" class="blob-code blob-code-inner js-file-line"><span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L12" class="blob-num js-line-number" data-line-number="12"></td>
        <td id="LC12" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#</span></td>
      </tr>
      <tr>
        <td id="L13" class="blob-num js-line-number" data-line-number="13"></td>
        <td id="LC13" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># This code is implemented using Mark Pilgrim&#39;s code as a guideline:</span></td>
      </tr>
      <tr>
        <td id="L14" class="blob-num js-line-number" data-line-number="14"></td>
        <td id="LC14" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   http://www.faqs.org/docs/diveintopython/kgp_divein.html</span></td>
      </tr>
      <tr>
        <td id="L15" class="blob-num js-line-number" data-line-number="15"></td>
        <td id="LC15" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#</span></td>
      </tr>
      <tr>
        <td id="L16" class="blob-num js-line-number" data-line-number="16"></td>
        <td id="LC16" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># Original Author: Prabhu Ramachandran</span></td>
      </tr>
      <tr>
        <td id="L17" class="blob-num js-line-number" data-line-number="17"></td>
        <td id="LC17" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># Modified by:     Michael Thon (June 2015)</span></td>
      </tr>
      <tr>
        <td id="L18" class="blob-num js-line-number" data-line-number="18"></td>
        <td id="LC18" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># License: BSD style</span></td>
      </tr>
      <tr>
        <td id="L19" class="blob-num js-line-number" data-line-number="19"></td>
        <td id="LC19" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#</span></td>
      </tr>
      <tr>
        <td id="L20" class="blob-num js-line-number" data-line-number="20"></td>
        <td id="LC20" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># Thanks:</span></td>
      </tr>
      <tr>
        <td id="L21" class="blob-num js-line-number" data-line-number="21"></td>
        <td id="LC21" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   Johan Hake:  the include_function_definition feature</span></td>
      </tr>
      <tr>
        <td id="L22" class="blob-num js-line-number" data-line-number="22"></td>
        <td id="LC22" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   Bill Spotz:  bug reports and testing.</span></td>
      </tr>
      <tr>
        <td id="L23" class="blob-num js-line-number" data-line-number="23"></td>
        <td id="LC23" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   Sebastian Henschel:   Misc. enhancements.</span></td>
      </tr>
      <tr>
        <td id="L24" class="blob-num js-line-number" data-line-number="24"></td>
        <td id="LC24" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#</span></td>
      </tr>
      <tr>
        <td id="L25" class="blob-num js-line-number" data-line-number="25"></td>
        <td id="LC25" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># Changes:</span></td>
      </tr>
      <tr>
        <td id="L26" class="blob-num js-line-number" data-line-number="26"></td>
        <td id="LC26" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># June 2015 (Michael Thon):</span></td>
      </tr>
      <tr>
        <td id="L27" class="blob-num js-line-number" data-line-number="27"></td>
        <td id="LC27" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   - class documentation:</span></td>
      </tr>
      <tr>
        <td id="L28" class="blob-num js-line-number" data-line-number="28"></td>
        <td id="LC28" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     -c: add constructor call signatures and a &quot;Constructors&quot; section</span></td>
      </tr>
      <tr>
        <td id="L29" class="blob-num js-line-number" data-line-number="29"></td>
        <td id="LC29" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#         collecting the respective docs (e.g. for python)</span></td>
      </tr>
      <tr>
        <td id="L30" class="blob-num js-line-number" data-line-number="30"></td>
        <td id="LC30" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     -a: add &quot;Attributes&quot; section collecting the documentation for member</span></td>
      </tr>
      <tr>
        <td id="L31" class="blob-num js-line-number" data-line-number="31"></td>
        <td id="LC31" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#         variables (e.g. for python)</span></td>
      </tr>
      <tr>
        <td id="L32" class="blob-num js-line-number" data-line-number="32"></td>
        <td id="LC32" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   - overloaded functions:</span></td>
      </tr>
      <tr>
        <td id="L33" class="blob-num js-line-number" data-line-number="33"></td>
        <td id="LC33" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     -o: collect all documentation into one &quot;Overloaded function&quot; section</span></td>
      </tr>
      <tr>
        <td id="L34" class="blob-num js-line-number" data-line-number="34"></td>
        <td id="LC34" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   - option to include function definition / signature renamed to -f</span></td>
      </tr>
      <tr>
        <td id="L35" class="blob-num js-line-number" data-line-number="35"></td>
        <td id="LC35" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#   - formatting:</span></td>
      </tr>
      <tr>
        <td id="L36" class="blob-num js-line-number" data-line-number="36"></td>
        <td id="LC36" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + included function signatures slightly reformatted</span></td>
      </tr>
      <tr>
        <td id="L37" class="blob-num js-line-number" data-line-number="37"></td>
        <td id="LC37" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + option (-t) to turn off/on type information for funciton signatures</span></td>
      </tr>
      <tr>
        <td id="L38" class="blob-num js-line-number" data-line-number="38"></td>
        <td id="LC38" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + lists (incl. nested and ordered)</span></td>
      </tr>
      <tr>
        <td id="L39" class="blob-num js-line-number" data-line-number="39"></td>
        <td id="LC39" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + attempt to produce docstrings that render nicely as markdown</span></td>
      </tr>
      <tr>
        <td id="L40" class="blob-num js-line-number" data-line-number="40"></td>
        <td id="LC40" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + translate code, emphasis, bold, linebreak, hruler, blockquote,</span></td>
      </tr>
      <tr>
        <td id="L41" class="blob-num js-line-number" data-line-number="41"></td>
        <td id="LC41" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#       verbatim, heading tags to markdown</span></td>
      </tr>
      <tr>
        <td id="L42" class="blob-num js-line-number" data-line-number="42"></td>
        <td id="LC42" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#     + new text-wrapping and option -w to specify the text width</span></td>
      </tr>
      <tr>
        <td id="L43" class="blob-num js-line-number" data-line-number="43"></td>
        <td id="LC43" class="blob-code blob-code-inner js-file-line"><span class="pl-c">#</span></td>
      </tr>
      <tr>
        <td id="L44" class="blob-num js-line-number" data-line-number="44"></td>
        <td id="LC44" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L45" class="blob-num js-line-number" data-line-number="45"></td>
        <td id="LC45" class="blob-code blob-code-inner js-file-line"><span class="pl-k">from</span> xml.dom <span class="pl-k">import</span> minidom</td>
      </tr>
      <tr>
        <td id="L46" class="blob-num js-line-number" data-line-number="46"></td>
        <td id="LC46" class="blob-code blob-code-inner js-file-line"><span class="pl-k">import</span> re</td>
      </tr>
      <tr>
        <td id="L47" class="blob-num js-line-number" data-line-number="47"></td>
        <td id="LC47" class="blob-code blob-code-inner js-file-line"><span class="pl-k">import</span> textwrap</td>
      </tr>
      <tr>
        <td id="L48" class="blob-num js-line-number" data-line-number="48"></td>
        <td id="LC48" class="blob-code blob-code-inner js-file-line"><span class="pl-k">import</span> sys</td>
      </tr>
      <tr>
        <td id="L49" class="blob-num js-line-number" data-line-number="49"></td>
        <td id="LC49" class="blob-code blob-code-inner js-file-line"><span class="pl-k">import</span> os.path</td>
      </tr>
      <tr>
        <td id="L50" class="blob-num js-line-number" data-line-number="50"></td>
        <td id="LC50" class="blob-code blob-code-inner js-file-line"><span class="pl-k">import</span> optparse</td>
      </tr>
      <tr>
        <td id="L51" class="blob-num js-line-number" data-line-number="51"></td>
        <td id="LC51" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L52" class="blob-num js-line-number" data-line-number="52"></td>
        <td id="LC52" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L53" class="blob-num js-line-number" data-line-number="53"></td>
        <td id="LC53" class="blob-code blob-code-inner js-file-line"><span class="pl-k">def</span> <span class="pl-en">my_open_read</span>(<span class="pl-smi">source</span>):</td>
      </tr>
      <tr>
        <td id="L54" class="blob-num js-line-number" data-line-number="54"></td>
        <td id="LC54" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> <span class="pl-c1">hasattr</span>(source, <span class="pl-s"><span class="pl-pds">&quot;</span>read<span class="pl-pds">&quot;</span></span>):</td>
      </tr>
      <tr>
        <td id="L55" class="blob-num js-line-number" data-line-number="55"></td>
        <td id="LC55" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> source</td>
      </tr>
      <tr>
        <td id="L56" class="blob-num js-line-number" data-line-number="56"></td>
        <td id="LC56" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L57" class="blob-num js-line-number" data-line-number="57"></td>
        <td id="LC57" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">try</span>:</td>
      </tr>
      <tr>
        <td id="L58" class="blob-num js-line-number" data-line-number="58"></td>
        <td id="LC58" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span> <span class="pl-c1">open</span>(source, <span class="pl-smi">encoding</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>utf-8<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L59" class="blob-num js-line-number" data-line-number="59"></td>
        <td id="LC59" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">except</span> <span class="pl-c1">TypeError</span>:</td>
      </tr>
      <tr>
        <td id="L60" class="blob-num js-line-number" data-line-number="60"></td>
        <td id="LC60" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span> <span class="pl-c1">open</span>(source)</td>
      </tr>
      <tr>
        <td id="L61" class="blob-num js-line-number" data-line-number="61"></td>
        <td id="LC61" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L62" class="blob-num js-line-number" data-line-number="62"></td>
        <td id="LC62" class="blob-code blob-code-inner js-file-line"><span class="pl-k">def</span> <span class="pl-en">my_open_write</span>(<span class="pl-smi">dest</span>):</td>
      </tr>
      <tr>
        <td id="L63" class="blob-num js-line-number" data-line-number="63"></td>
        <td id="LC63" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> <span class="pl-c1">hasattr</span>(dest, <span class="pl-s"><span class="pl-pds">&quot;</span>write<span class="pl-pds">&quot;</span></span>):</td>
      </tr>
      <tr>
        <td id="L64" class="blob-num js-line-number" data-line-number="64"></td>
        <td id="LC64" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> dest</td>
      </tr>
      <tr>
        <td id="L65" class="blob-num js-line-number" data-line-number="65"></td>
        <td id="LC65" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L66" class="blob-num js-line-number" data-line-number="66"></td>
        <td id="LC66" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">try</span>:</td>
      </tr>
      <tr>
        <td id="L67" class="blob-num js-line-number" data-line-number="67"></td>
        <td id="LC67" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span> <span class="pl-c1">open</span>(dest, <span class="pl-s"><span class="pl-pds">&#39;</span>w<span class="pl-pds">&#39;</span></span>, <span class="pl-smi">encoding</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>utf-8<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L68" class="blob-num js-line-number" data-line-number="68"></td>
        <td id="LC68" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">except</span> <span class="pl-c1">TypeError</span>:</td>
      </tr>
      <tr>
        <td id="L69" class="blob-num js-line-number" data-line-number="69"></td>
        <td id="LC69" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span> <span class="pl-c1">open</span>(dest, <span class="pl-s"><span class="pl-pds">&#39;</span>w<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L70" class="blob-num js-line-number" data-line-number="70"></td>
        <td id="LC70" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L71" class="blob-num js-line-number" data-line-number="71"></td>
        <td id="LC71" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Text handling:</span></td>
      </tr>
      <tr>
        <td id="L72" class="blob-num js-line-number" data-line-number="72"></td>
        <td id="LC72" class="blob-code blob-code-inner js-file-line"><span class="pl-k">def</span> <span class="pl-en">shift</span>(<span class="pl-smi">txt</span>, <span class="pl-smi">indent</span> <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>    <span class="pl-pds">&#39;</span></span>, <span class="pl-smi">prepend</span> <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L73" class="blob-num js-line-number" data-line-number="73"></td>
        <td id="LC73" class="blob-code blob-code-inner js-file-line">    <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Return a list corresponding to the lines of text in the `txt` list</span></td>
      </tr>
      <tr>
        <td id="L74" class="blob-num js-line-number" data-line-number="74"></td>
        <td id="LC74" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    indented by `indent`. Prepend instead the string given in `prepend` to the</span></td>
      </tr>
      <tr>
        <td id="L75" class="blob-num js-line-number" data-line-number="75"></td>
        <td id="LC75" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    beginning of the first line. Note that if len(prepend) &gt; len(indent), then</span></td>
      </tr>
      <tr>
        <td id="L76" class="blob-num js-line-number" data-line-number="76"></td>
        <td id="LC76" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    `prepend` will be truncated (doing better is tricky!). This preserves a </span></td>
      </tr>
      <tr>
        <td id="L77" class="blob-num js-line-number" data-line-number="77"></td>
        <td id="LC77" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    special &#39;&#39; entry at the end of `txt` (see `do_para` for the meaning).</span></td>
      </tr>
      <tr>
        <td id="L78" class="blob-num js-line-number" data-line-number="78"></td>
        <td id="LC78" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L79" class="blob-num js-line-number" data-line-number="79"></td>
        <td id="LC79" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> <span class="pl-c1">type</span>(indent) <span class="pl-k">is</span> <span class="pl-c1">int</span>:</td>
      </tr>
      <tr>
        <td id="L80" class="blob-num js-line-number" data-line-number="80"></td>
        <td id="LC80" class="blob-code blob-code-inner js-file-line">        indent <span class="pl-k">=</span> indent <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L81" class="blob-num js-line-number" data-line-number="81"></td>
        <td id="LC81" class="blob-code blob-code-inner js-file-line">    special_end <span class="pl-k">=</span> txt[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">==</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L82" class="blob-num js-line-number" data-line-number="82"></td>
        <td id="LC82" class="blob-code blob-code-inner js-file-line">    lines <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(txt).splitlines(<span class="pl-c1">True</span>)</td>
      </tr>
      <tr>
        <td id="L83" class="blob-num js-line-number" data-line-number="83"></td>
        <td id="LC83" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">for</span> i <span class="pl-k">in</span> <span class="pl-c1">range</span>(<span class="pl-c1">1</span>,<span class="pl-c1">len</span>(lines)):</td>
      </tr>
      <tr>
        <td id="L84" class="blob-num js-line-number" data-line-number="84"></td>
        <td id="LC84" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> lines[i].strip() <span class="pl-k">or</span> indent.strip():</td>
      </tr>
      <tr>
        <td id="L85" class="blob-num js-line-number" data-line-number="85"></td>
        <td id="LC85" class="blob-code blob-code-inner js-file-line">            lines[i] <span class="pl-k">=</span> indent <span class="pl-k">+</span> lines[i]</td>
      </tr>
      <tr>
        <td id="L86" class="blob-num js-line-number" data-line-number="86"></td>
        <td id="LC86" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> <span class="pl-k">not</span> lines:</td>
      </tr>
      <tr>
        <td id="L87" class="blob-num js-line-number" data-line-number="87"></td>
        <td id="LC87" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> prepend</td>
      </tr>
      <tr>
        <td id="L88" class="blob-num js-line-number" data-line-number="88"></td>
        <td id="LC88" class="blob-code blob-code-inner js-file-line">    prepend <span class="pl-k">=</span> prepend[:<span class="pl-c1">len</span>(indent)]</td>
      </tr>
      <tr>
        <td id="L89" class="blob-num js-line-number" data-line-number="89"></td>
        <td id="LC89" class="blob-code blob-code-inner js-file-line">    indent <span class="pl-k">=</span> indent[<span class="pl-c1">len</span>(prepend):]</td>
      </tr>
      <tr>
        <td id="L90" class="blob-num js-line-number" data-line-number="90"></td>
        <td id="LC90" class="blob-code blob-code-inner js-file-line">    lines[<span class="pl-c1">0</span>] <span class="pl-k">=</span> prepend <span class="pl-k">+</span> indent <span class="pl-k">+</span> lines[<span class="pl-c1">0</span>]</td>
      </tr>
      <tr>
        <td id="L91" class="blob-num js-line-number" data-line-number="91"></td>
        <td id="LC91" class="blob-code blob-code-inner js-file-line">    ret <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(lines)]</td>
      </tr>
      <tr>
        <td id="L92" class="blob-num js-line-number" data-line-number="92"></td>
        <td id="LC92" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> special_end:</td>
      </tr>
      <tr>
        <td id="L93" class="blob-num js-line-number" data-line-number="93"></td>
        <td id="LC93" class="blob-code blob-code-inner js-file-line">        ret.append(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L94" class="blob-num js-line-number" data-line-number="94"></td>
        <td id="LC94" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">return</span> ret</td>
      </tr>
      <tr>
        <td id="L95" class="blob-num js-line-number" data-line-number="95"></td>
        <td id="LC95" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L96" class="blob-num js-line-number" data-line-number="96"></td>
        <td id="LC96" class="blob-code blob-code-inner js-file-line"><span class="pl-k">class</span> <span class="pl-en">Doxy2SWIG</span>:</td>
      </tr>
      <tr>
        <td id="L97" class="blob-num js-line-number" data-line-number="97"></td>
        <td id="LC97" class="blob-code blob-code-inner js-file-line">    <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Converts Doxygen generated XML files into a file containing</span></td>
      </tr>
      <tr>
        <td id="L98" class="blob-num js-line-number" data-line-number="98"></td>
        <td id="LC98" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    docstrings that can be used by SWIG-1.3.x that have support for</span></td>
      </tr>
      <tr>
        <td id="L99" class="blob-num js-line-number" data-line-number="99"></td>
        <td id="LC99" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    feature(&quot;docstring&quot;).  Once the data is parsed it is stored in</span></td>
      </tr>
      <tr>
        <td id="L100" class="blob-num js-line-number" data-line-number="100"></td>
        <td id="LC100" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    self.pieces.</span></td>
      </tr>
      <tr>
        <td id="L101" class="blob-num js-line-number" data-line-number="101"></td>
        <td id="LC101" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L102" class="blob-num js-line-number" data-line-number="102"></td>
        <td id="LC102" class="blob-code blob-code-inner js-file-line"><span class="pl-s">    <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L103" class="blob-num js-line-number" data-line-number="103"></td>
        <td id="LC103" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L104" class="blob-num js-line-number" data-line-number="104"></td>
        <td id="LC104" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en"><span class="pl-c1">__init__</span></span>(<span class="pl-smi">self</span>, <span class="pl-smi">src</span>,</td>
      </tr>
      <tr>
        <td id="L105" class="blob-num js-line-number" data-line-number="105"></td>
        <td id="LC105" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">with_function_signature</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L106" class="blob-num js-line-number" data-line-number="106"></td>
        <td id="LC106" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">with_type_info</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L107" class="blob-num js-line-number" data-line-number="107"></td>
        <td id="LC107" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">with_constructor_list</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L108" class="blob-num js-line-number" data-line-number="108"></td>
        <td id="LC108" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">with_attribute_list</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L109" class="blob-num js-line-number" data-line-number="109"></td>
        <td id="LC109" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">with_overloaded_functions</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L110" class="blob-num js-line-number" data-line-number="110"></td>
        <td id="LC110" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">textwidth</span> <span class="pl-k">=</span> <span class="pl-c1">80</span>,</td>
      </tr>
      <tr>
        <td id="L111" class="blob-num js-line-number" data-line-number="111"></td>
        <td id="LC111" class="blob-code blob-code-inner js-file-line">                 <span class="pl-smi">quiet</span> <span class="pl-k">=</span> <span class="pl-c1">False</span>):</td>
      </tr>
      <tr>
        <td id="L112" class="blob-num js-line-number" data-line-number="112"></td>
        <td id="LC112" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Initialize the instance given a source object.  `src` can</span></td>
      </tr>
      <tr>
        <td id="L113" class="blob-num js-line-number" data-line-number="113"></td>
        <td id="LC113" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        be a file or filename.  If you do not want to include function</span></td>
      </tr>
      <tr>
        <td id="L114" class="blob-num js-line-number" data-line-number="114"></td>
        <td id="LC114" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        definitions from doxygen then set</span></td>
      </tr>
      <tr>
        <td id="L115" class="blob-num js-line-number" data-line-number="115"></td>
        <td id="LC115" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `include_function_definition` to `False`.  This is handy since</span></td>
      </tr>
      <tr>
        <td id="L116" class="blob-num js-line-number" data-line-number="116"></td>
        <td id="LC116" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        this allows you to use the swig generated function definition</span></td>
      </tr>
      <tr>
        <td id="L117" class="blob-num js-line-number" data-line-number="117"></td>
        <td id="LC117" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        using <span class="pl-c1">%f</span>eature(&quot;autodoc&quot;, [0,1]).</span></td>
      </tr>
      <tr>
        <td id="L118" class="blob-num js-line-number" data-line-number="118"></td>
        <td id="LC118" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L119" class="blob-num js-line-number" data-line-number="119"></td>
        <td id="LC119" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L120" class="blob-num js-line-number" data-line-number="120"></td>
        <td id="LC120" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># options:</span></td>
      </tr>
      <tr>
        <td id="L121" class="blob-num js-line-number" data-line-number="121"></td>
        <td id="LC121" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.with_function_signature <span class="pl-k">=</span> with_function_signature</td>
      </tr>
      <tr>
        <td id="L122" class="blob-num js-line-number" data-line-number="122"></td>
        <td id="LC122" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.with_type_info <span class="pl-k">=</span> with_type_info</td>
      </tr>
      <tr>
        <td id="L123" class="blob-num js-line-number" data-line-number="123"></td>
        <td id="LC123" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.with_constructor_list <span class="pl-k">=</span> with_constructor_list</td>
      </tr>
      <tr>
        <td id="L124" class="blob-num js-line-number" data-line-number="124"></td>
        <td id="LC124" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.with_attribute_list <span class="pl-k">=</span> with_attribute_list</td>
      </tr>
      <tr>
        <td id="L125" class="blob-num js-line-number" data-line-number="125"></td>
        <td id="LC125" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.with_overloaded_functions <span class="pl-k">=</span> with_overloaded_functions</td>
      </tr>
      <tr>
        <td id="L126" class="blob-num js-line-number" data-line-number="126"></td>
        <td id="LC126" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.textwidth <span class="pl-k">=</span> textwidth</td>
      </tr>
      <tr>
        <td id="L127" class="blob-num js-line-number" data-line-number="127"></td>
        <td id="LC127" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.quiet <span class="pl-k">=</span> quiet</td>
      </tr>
      <tr>
        <td id="L128" class="blob-num js-line-number" data-line-number="128"></td>
        <td id="LC128" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L129" class="blob-num js-line-number" data-line-number="129"></td>
        <td id="LC129" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># state:</span></td>
      </tr>
      <tr>
        <td id="L130" class="blob-num js-line-number" data-line-number="130"></td>
        <td id="LC130" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.indent <span class="pl-k">=</span> <span class="pl-c1">0</span></td>
      </tr>
      <tr>
        <td id="L131" class="blob-num js-line-number" data-line-number="131"></td>
        <td id="LC131" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L132" class="blob-num js-line-number" data-line-number="132"></td>
        <td id="LC132" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L133" class="blob-num js-line-number" data-line-number="133"></td>
        <td id="LC133" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L134" class="blob-num js-line-number" data-line-number="134"></td>
        <td id="LC134" class="blob-code blob-code-inner js-file-line">        f <span class="pl-k">=</span> my_open_read(src)</td>
      </tr>
      <tr>
        <td id="L135" class="blob-num js-line-number" data-line-number="135"></td>
        <td id="LC135" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.my_dir <span class="pl-k">=</span> os.path.dirname(f.name)</td>
      </tr>
      <tr>
        <td id="L136" class="blob-num js-line-number" data-line-number="136"></td>
        <td id="LC136" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.xmldoc <span class="pl-k">=</span> minidom.parse(f).documentElement</td>
      </tr>
      <tr>
        <td id="L137" class="blob-num js-line-number" data-line-number="137"></td>
        <td id="LC137" class="blob-code blob-code-inner js-file-line">        f.close()</td>
      </tr>
      <tr>
        <td id="L138" class="blob-num js-line-number" data-line-number="138"></td>
        <td id="LC138" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L139" class="blob-num js-line-number" data-line-number="139"></td>
        <td id="LC139" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces.append(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>// File: <span class="pl-c1">%s</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span> <span class="pl-k">%</span></td>
      </tr>
      <tr>
        <td id="L140" class="blob-num js-line-number" data-line-number="140"></td>
        <td id="LC140" class="blob-code blob-code-inner js-file-line">                           os.path.basename(f.name))</td>
      </tr>
      <tr>
        <td id="L141" class="blob-num js-line-number" data-line-number="141"></td>
        <td id="LC141" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L142" class="blob-num js-line-number" data-line-number="142"></td>
        <td id="LC142" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.space_re <span class="pl-k">=</span> re.compile(<span class="pl-s"><span class="pl-k">r</span><span class="pl-pds">&#39;</span><span class="pl-c1">\s</span><span class="pl-k">+</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L143" class="blob-num js-line-number" data-line-number="143"></td>
        <td id="LC143" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.lead_spc <span class="pl-k">=</span> re.compile(<span class="pl-s"><span class="pl-k">r</span><span class="pl-pds">&#39;</span><span class="pl-k">^</span>(%feature<span class="pl-c1">\S</span><span class="pl-k">+</span><span class="pl-c1">\s</span><span class="pl-k">+</span><span class="pl-c1">\S</span><span class="pl-k">+</span><span class="pl-c1">\s</span><span class="pl-k">*?</span>)&quot;<span class="pl-c1">\s</span><span class="pl-k">+</span>(<span class="pl-c1">\S</span>)<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L144" class="blob-num js-line-number" data-line-number="144"></td>
        <td id="LC144" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.multi <span class="pl-k">=</span> <span class="pl-c1">0</span></td>
      </tr>
      <tr>
        <td id="L145" class="blob-num js-line-number" data-line-number="145"></td>
        <td id="LC145" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.ignores <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>inheritancegraph<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>param<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>listofallmembers<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L146" class="blob-num js-line-number" data-line-number="146"></td>
        <td id="LC146" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>innerclass<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>declname<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>incdepgraph<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L147" class="blob-num js-line-number" data-line-number="147"></td>
        <td id="LC147" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>invincdepgraph<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>programlisting<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>type<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L148" class="blob-num js-line-number" data-line-number="148"></td>
        <td id="LC148" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>references<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>referencedby<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>location<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L149" class="blob-num js-line-number" data-line-number="149"></td>
        <td id="LC149" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>collaborationgraph<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>reimplements<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L150" class="blob-num js-line-number" data-line-number="150"></td>
        <td id="LC150" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>reimplementedby<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>derivedcompoundref<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L151" class="blob-num js-line-number" data-line-number="151"></td>
        <td id="LC151" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>basecompoundref<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L152" class="blob-num js-line-number" data-line-number="152"></td>
        <td id="LC152" class="blob-code blob-code-inner js-file-line">                        <span class="pl-s"><span class="pl-pds">&#39;</span>argsstring<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>exceptions<span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L153" class="blob-num js-line-number" data-line-number="153"></td>
        <td id="LC153" class="blob-code blob-code-inner js-file-line">        <span class="pl-c">#self.generics = []</span></td>
      </tr>
      <tr>
        <td id="L154" class="blob-num js-line-number" data-line-number="154"></td>
        <td id="LC154" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L155" class="blob-num js-line-number" data-line-number="155"></td>
        <td id="LC155" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">generate</span>(<span class="pl-smi">self</span>):</td>
      </tr>
      <tr>
        <td id="L156" class="blob-num js-line-number" data-line-number="156"></td>
        <td id="LC156" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parses the file set in the initialization.  The resulting</span></td>
      </tr>
      <tr>
        <td id="L157" class="blob-num js-line-number" data-line-number="157"></td>
        <td id="LC157" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        data is stored in `self.pieces`.</span></td>
      </tr>
      <tr>
        <td id="L158" class="blob-num js-line-number" data-line-number="158"></td>
        <td id="LC158" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L159" class="blob-num js-line-number" data-line-number="159"></td>
        <td id="LC159" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L160" class="blob-num js-line-number" data-line-number="160"></td>
        <td id="LC160" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.parse(<span class="pl-v">self</span>.xmldoc)</td>
      </tr>
      <tr>
        <td id="L161" class="blob-num js-line-number" data-line-number="161"></td>
        <td id="LC161" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L162" class="blob-num js-line-number" data-line-number="162"></td>
        <td id="LC162" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">write</span>(<span class="pl-smi">self</span>, <span class="pl-smi">fname</span>):</td>
      </tr>
      <tr>
        <td id="L163" class="blob-num js-line-number" data-line-number="163"></td>
        <td id="LC163" class="blob-code blob-code-inner js-file-line">        o <span class="pl-k">=</span> my_open_write(fname)</td>
      </tr>
      <tr>
        <td id="L164" class="blob-num js-line-number" data-line-number="164"></td>
        <td id="LC164" class="blob-code blob-code-inner js-file-line">        o.write(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces))</td>
      </tr>
      <tr>
        <td id="L165" class="blob-num js-line-number" data-line-number="165"></td>
        <td id="LC165" class="blob-code blob-code-inner js-file-line">        o.write(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L166" class="blob-num js-line-number" data-line-number="166"></td>
        <td id="LC166" class="blob-code blob-code-inner js-file-line">        o.close()</td>
      </tr>
      <tr>
        <td id="L167" class="blob-num js-line-number" data-line-number="167"></td>
        <td id="LC167" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L168" class="blob-num js-line-number" data-line-number="168"></td>
        <td id="LC168" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">parse</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L169" class="blob-num js-line-number" data-line-number="169"></td>
        <td id="LC169" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parse a given node.  This function in turn calls the</span></td>
      </tr>
      <tr>
        <td id="L170" class="blob-num js-line-number" data-line-number="170"></td>
        <td id="LC170" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `parse_&lt;nodeType&gt;` functions which handle the respective</span></td>
      </tr>
      <tr>
        <td id="L171" class="blob-num js-line-number" data-line-number="171"></td>
        <td id="LC171" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        nodes.</span></td>
      </tr>
      <tr>
        <td id="L172" class="blob-num js-line-number" data-line-number="172"></td>
        <td id="LC172" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L173" class="blob-num js-line-number" data-line-number="173"></td>
        <td id="LC173" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L174" class="blob-num js-line-number" data-line-number="174"></td>
        <td id="LC174" class="blob-code blob-code-inner js-file-line">        pm <span class="pl-k">=</span> <span class="pl-c1">getattr</span>(<span class="pl-v">self</span>, <span class="pl-s"><span class="pl-pds">&quot;</span>parse_<span class="pl-c1">%s</span><span class="pl-pds">&quot;</span></span> <span class="pl-k">%</span> node.<span class="pl-c1">__class__</span>.<span class="pl-c1">__name__</span>)</td>
      </tr>
      <tr>
        <td id="L175" class="blob-num js-line-number" data-line-number="175"></td>
        <td id="LC175" class="blob-code blob-code-inner js-file-line">        pm(node)</td>
      </tr>
      <tr>
        <td id="L176" class="blob-num js-line-number" data-line-number="176"></td>
        <td id="LC176" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L177" class="blob-num js-line-number" data-line-number="177"></td>
        <td id="LC177" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">parse_Document</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L178" class="blob-num js-line-number" data-line-number="178"></td>
        <td id="LC178" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.parse(node.documentElement)</td>
      </tr>
      <tr>
        <td id="L179" class="blob-num js-line-number" data-line-number="179"></td>
        <td id="LC179" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L180" class="blob-num js-line-number" data-line-number="180"></td>
        <td id="LC180" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">parse_Text</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L181" class="blob-num js-line-number" data-line-number="181"></td>
        <td id="LC181" class="blob-code blob-code-inner js-file-line">        txt <span class="pl-k">=</span> node.data</td>
      </tr>
      <tr>
        <td id="L182" class="blob-num js-line-number" data-line-number="182"></td>
        <td id="LC182" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> txt <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L183" class="blob-num js-line-number" data-line-number="183"></td>
        <td id="LC183" class="blob-code blob-code-inner js-file-line">            <span class="pl-c"># this can happen when two tags follow in a text, e.g.,</span></td>
      </tr>
      <tr>
        <td id="L184" class="blob-num js-line-number" data-line-number="184"></td>
        <td id="LC184" class="blob-code blob-code-inner js-file-line">            <span class="pl-c"># &quot; ...&lt;/emph&gt; &lt;formaula&gt;$...&quot; etc.</span></td>
      </tr>
      <tr>
        <td id="L185" class="blob-num js-line-number" data-line-number="185"></td>
        <td id="LC185" class="blob-code blob-code-inner js-file-line">            <span class="pl-c"># here we want to keep the space.</span></td>
      </tr>
      <tr>
        <td id="L186" class="blob-num js-line-number" data-line-number="186"></td>
        <td id="LC186" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(txt)</td>
      </tr>
      <tr>
        <td id="L187" class="blob-num js-line-number" data-line-number="187"></td>
        <td id="LC187" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L188" class="blob-num js-line-number" data-line-number="188"></td>
        <td id="LC188" class="blob-code blob-code-inner js-file-line">        txt <span class="pl-k">=</span> txt.replace(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\\</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-k">r</span><span class="pl-pds">&#39;</span><span class="pl-cce">\\</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L189" class="blob-num js-line-number" data-line-number="189"></td>
        <td id="LC189" class="blob-code blob-code-inner js-file-line">        txt <span class="pl-k">=</span> txt.replace(<span class="pl-s"><span class="pl-pds">&#39;</span>&quot;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-k">r</span><span class="pl-pds">&#39;</span><span class="pl-cce">\&quot;</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L190" class="blob-num js-line-number" data-line-number="190"></td>
        <td id="LC190" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># ignore pure whitespace</span></td>
      </tr>
      <tr>
        <td id="L191" class="blob-num js-line-number" data-line-number="191"></td>
        <td id="LC191" class="blob-code blob-code-inner js-file-line">        m <span class="pl-k">=</span> <span class="pl-v">self</span>.space_re.match(txt)</td>
      </tr>
      <tr>
        <td id="L192" class="blob-num js-line-number" data-line-number="192"></td>
        <td id="LC192" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> m <span class="pl-k">and</span> <span class="pl-c1">len</span>(m.group()) <span class="pl-k">==</span> <span class="pl-c1">len</span>(txt):</td>
      </tr>
      <tr>
        <td id="L193" class="blob-num js-line-number" data-line-number="193"></td>
        <td id="LC193" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">pass</span></td>
      </tr>
      <tr>
        <td id="L194" class="blob-num js-line-number" data-line-number="194"></td>
        <td id="LC194" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L195" class="blob-num js-line-number" data-line-number="195"></td>
        <td id="LC195" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(txt)</td>
      </tr>
      <tr>
        <td id="L196" class="blob-num js-line-number" data-line-number="196"></td>
        <td id="LC196" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L197" class="blob-num js-line-number" data-line-number="197"></td>
        <td id="LC197" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">parse_Comment</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L198" class="blob-num js-line-number" data-line-number="198"></td>
        <td id="LC198" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parse a `COMMENT_NODE`.  This does nothing for now.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L199" class="blob-num js-line-number" data-line-number="199"></td>
        <td id="LC199" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L200" class="blob-num js-line-number" data-line-number="200"></td>
        <td id="LC200" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L201" class="blob-num js-line-number" data-line-number="201"></td>
        <td id="LC201" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">parse_Element</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L202" class="blob-num js-line-number" data-line-number="202"></td>
        <td id="LC202" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parse an `ELEMENT_NODE`.  This calls specific</span></td>
      </tr>
      <tr>
        <td id="L203" class="blob-num js-line-number" data-line-number="203"></td>
        <td id="LC203" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `do_&lt;tagName&gt;` handers for different elements.  If no handler</span></td>
      </tr>
      <tr>
        <td id="L204" class="blob-num js-line-number" data-line-number="204"></td>
        <td id="LC204" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        is available the `subnode_parse` method is called.  All</span></td>
      </tr>
      <tr>
        <td id="L205" class="blob-num js-line-number" data-line-number="205"></td>
        <td id="LC205" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        tagNames specified in `self.ignores` are simply ignored.</span></td>
      </tr>
      <tr>
        <td id="L206" class="blob-num js-line-number" data-line-number="206"></td>
        <td id="LC206" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L207" class="blob-num js-line-number" data-line-number="207"></td>
        <td id="LC207" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L208" class="blob-num js-line-number" data-line-number="208"></td>
        <td id="LC208" class="blob-code blob-code-inner js-file-line">        name <span class="pl-k">=</span> node.tagName</td>
      </tr>
      <tr>
        <td id="L209" class="blob-num js-line-number" data-line-number="209"></td>
        <td id="LC209" class="blob-code blob-code-inner js-file-line">        ignores <span class="pl-k">=</span> <span class="pl-v">self</span>.ignores</td>
      </tr>
      <tr>
        <td id="L210" class="blob-num js-line-number" data-line-number="210"></td>
        <td id="LC210" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> name <span class="pl-k">in</span> ignores:</td>
      </tr>
      <tr>
        <td id="L211" class="blob-num js-line-number" data-line-number="211"></td>
        <td id="LC211" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L212" class="blob-num js-line-number" data-line-number="212"></td>
        <td id="LC212" class="blob-code blob-code-inner js-file-line">        attr <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&quot;</span>do_<span class="pl-c1">%s</span><span class="pl-pds">&quot;</span></span> <span class="pl-k">%</span> name</td>
      </tr>
      <tr>
        <td id="L213" class="blob-num js-line-number" data-line-number="213"></td>
        <td id="LC213" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">hasattr</span>(<span class="pl-v">self</span>, attr):</td>
      </tr>
      <tr>
        <td id="L214" class="blob-num js-line-number" data-line-number="214"></td>
        <td id="LC214" class="blob-code blob-code-inner js-file-line">            handlerMethod <span class="pl-k">=</span> <span class="pl-c1">getattr</span>(<span class="pl-v">self</span>, attr)</td>
      </tr>
      <tr>
        <td id="L215" class="blob-num js-line-number" data-line-number="215"></td>
        <td id="LC215" class="blob-code blob-code-inner js-file-line">            handlerMethod(node)</td>
      </tr>
      <tr>
        <td id="L216" class="blob-num js-line-number" data-line-number="216"></td>
        <td id="LC216" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L217" class="blob-num js-line-number" data-line-number="217"></td>
        <td id="LC217" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L218" class="blob-num js-line-number" data-line-number="218"></td>
        <td id="LC218" class="blob-code blob-code-inner js-file-line">            <span class="pl-c">#if name not in self.generics: self.generics.append(name)</span></td>
      </tr>
      <tr>
        <td id="L219" class="blob-num js-line-number" data-line-number="219"></td>
        <td id="LC219" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L220" class="blob-num js-line-number" data-line-number="220"></td>
        <td id="LC220" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Special format parsing</span></td>
      </tr>
      <tr>
        <td id="L221" class="blob-num js-line-number" data-line-number="221"></td>
        <td id="LC221" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">subnode_parse</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>, <span class="pl-smi">pieces</span><span class="pl-k">=</span><span class="pl-c1">None</span>, <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">0</span>, <span class="pl-smi">ignore</span><span class="pl-k">=</span>[], <span class="pl-smi">restrict</span><span class="pl-k">=</span><span class="pl-c1">None</span>):</td>
      </tr>
      <tr>
        <td id="L222" class="blob-num js-line-number" data-line-number="222"></td>
        <td id="LC222" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parse the subnodes of a given node. Subnodes with tags in the</span></td>
      </tr>
      <tr>
        <td id="L223" class="blob-num js-line-number" data-line-number="223"></td>
        <td id="LC223" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `ignore` list are ignored. If pieces is given, use this as target for</span></td>
      </tr>
      <tr>
        <td id="L224" class="blob-num js-line-number" data-line-number="224"></td>
        <td id="LC224" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        the parse results instead of self.pieces. Indent all lines by the amount</span></td>
      </tr>
      <tr>
        <td id="L225" class="blob-num js-line-number" data-line-number="225"></td>
        <td id="LC225" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        given in `indent`. Note that the initial content in `pieces` is not</span></td>
      </tr>
      <tr>
        <td id="L226" class="blob-num js-line-number" data-line-number="226"></td>
        <td id="LC226" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        indented. The final result is in any case added to self.pieces.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L227" class="blob-num js-line-number" data-line-number="227"></td>
        <td id="LC227" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> pieces <span class="pl-k">is</span> <span class="pl-k">not</span> <span class="pl-c1">None</span>:</td>
      </tr>
      <tr>
        <td id="L228" class="blob-num js-line-number" data-line-number="228"></td>
        <td id="LC228" class="blob-code blob-code-inner js-file-line">            old_pieces, <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces, pieces</td>
      </tr>
      <tr>
        <td id="L229" class="blob-num js-line-number" data-line-number="229"></td>
        <td id="LC229" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L230" class="blob-num js-line-number" data-line-number="230"></td>
        <td id="LC230" class="blob-code blob-code-inner js-file-line">            old_pieces <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L231" class="blob-num js-line-number" data-line-number="231"></td>
        <td id="LC231" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">type</span>(indent) <span class="pl-k">is</span> <span class="pl-c1">int</span>:</td>
      </tr>
      <tr>
        <td id="L232" class="blob-num js-line-number" data-line-number="232"></td>
        <td id="LC232" class="blob-code blob-code-inner js-file-line">            indent <span class="pl-k">=</span> indent <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L233" class="blob-num js-line-number" data-line-number="233"></td>
        <td id="LC233" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">len</span>(indent) <span class="pl-k">&gt;</span> <span class="pl-c1">0</span>:</td>
      </tr>
      <tr>
        <td id="L234" class="blob-num js-line-number" data-line-number="234"></td>
        <td id="LC234" class="blob-code blob-code-inner js-file-line">            pieces <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces)</td>
      </tr>
      <tr>
        <td id="L235" class="blob-num js-line-number" data-line-number="235"></td>
        <td id="LC235" class="blob-code blob-code-inner js-file-line">            i_piece <span class="pl-k">=</span> pieces[:<span class="pl-c1">len</span>(indent)]</td>
      </tr>
      <tr>
        <td id="L236" class="blob-num js-line-number" data-line-number="236"></td>
        <td id="LC236" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">==</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L237" class="blob-num js-line-number" data-line-number="237"></td>
        <td id="LC237" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> [pieces[<span class="pl-c1">len</span>(indent):]] <span class="pl-k">+</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L238" class="blob-num js-line-number" data-line-number="238"></td>
        <td id="LC238" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">elif</span> <span class="pl-v">self</span>.pieces <span class="pl-k">!=</span> []:</td>
      </tr>
      <tr>
        <td id="L239" class="blob-num js-line-number" data-line-number="239"></td>
        <td id="LC239" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> [pieces[<span class="pl-c1">len</span>(indent):]]</td>
      </tr>
      <tr>
        <td id="L240" class="blob-num js-line-number" data-line-number="240"></td>
        <td id="LC240" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.indent <span class="pl-k">+=</span> <span class="pl-c1">len</span>(indent)</td>
      </tr>
      <tr>
        <td id="L241" class="blob-num js-line-number" data-line-number="241"></td>
        <td id="LC241" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> node.childNodes:</td>
      </tr>
      <tr>
        <td id="L242" class="blob-num js-line-number" data-line-number="242"></td>
        <td id="LC242" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> restrict <span class="pl-k">is</span> <span class="pl-k">not</span> <span class="pl-c1">None</span>:</td>
      </tr>
      <tr>
        <td id="L243" class="blob-num js-line-number" data-line-number="243"></td>
        <td id="LC243" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">if</span> n.nodeType <span class="pl-k">==</span> n.ELEMENT_NODE <span class="pl-k">and</span> n.tagName <span class="pl-k">in</span> restrict:</td>
      </tr>
      <tr>
        <td id="L244" class="blob-num js-line-number" data-line-number="244"></td>
        <td id="LC244" class="blob-code blob-code-inner js-file-line">                    <span class="pl-v">self</span>.parse(n)</td>
      </tr>
      <tr>
        <td id="L245" class="blob-num js-line-number" data-line-number="245"></td>
        <td id="LC245" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">elif</span> n.nodeType <span class="pl-k">!=</span> n.ELEMENT_NODE <span class="pl-k">or</span> n.tagName <span class="pl-k">not</span> <span class="pl-k">in</span> ignore:</td>
      </tr>
      <tr>
        <td id="L246" class="blob-num js-line-number" data-line-number="246"></td>
        <td id="LC246" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.parse(n)</td>
      </tr>
      <tr>
        <td id="L247" class="blob-num js-line-number" data-line-number="247"></td>
        <td id="LC247" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">len</span>(indent) <span class="pl-k">&gt;</span> <span class="pl-c1">0</span>:</td>
      </tr>
      <tr>
        <td id="L248" class="blob-num js-line-number" data-line-number="248"></td>
        <td id="LC248" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> shift(<span class="pl-v">self</span>.pieces, indent, i_piece)</td>
      </tr>
      <tr>
        <td id="L249" class="blob-num js-line-number" data-line-number="249"></td>
        <td id="LC249" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.indent <span class="pl-k">-=</span> <span class="pl-c1">len</span>(indent)</td>
      </tr>
      <tr>
        <td id="L250" class="blob-num js-line-number" data-line-number="250"></td>
        <td id="LC250" class="blob-code blob-code-inner js-file-line">        old_pieces.extend(<span class="pl-v">self</span>.pieces)</td>
      </tr>
      <tr>
        <td id="L251" class="blob-num js-line-number" data-line-number="251"></td>
        <td id="LC251" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> old_pieces</td>
      </tr>
      <tr>
        <td id="L252" class="blob-num js-line-number" data-line-number="252"></td>
        <td id="LC252" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L253" class="blob-num js-line-number" data-line-number="253"></td>
        <td id="LC253" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">surround_parse</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>, <span class="pl-smi">pre_char</span>, <span class="pl-smi">post_char</span>):</td>
      </tr>
      <tr>
        <td id="L254" class="blob-num js-line-number" data-line-number="254"></td>
        <td id="LC254" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Parse the subnodes of a given node. Subnodes with tags in the</span></td>
      </tr>
      <tr>
        <td id="L255" class="blob-num js-line-number" data-line-number="255"></td>
        <td id="LC255" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `ignore` list are ignored. Prepend `pre_char` and append `post_char` to</span></td>
      </tr>
      <tr>
        <td id="L256" class="blob-num js-line-number" data-line-number="256"></td>
        <td id="LC256" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        the output in self.pieces.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L257" class="blob-num js-line-number" data-line-number="257"></td>
        <td id="LC257" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(pre_char)</td>
      </tr>
      <tr>
        <td id="L258" class="blob-num js-line-number" data-line-number="258"></td>
        <td id="LC258" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L259" class="blob-num js-line-number" data-line-number="259"></td>
        <td id="LC259" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(post_char)</td>
      </tr>
      <tr>
        <td id="L260" class="blob-num js-line-number" data-line-number="260"></td>
        <td id="LC260" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L261" class="blob-num js-line-number" data-line-number="261"></td>
        <td id="LC261" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Helper functions</span></td>
      </tr>
      <tr>
        <td id="L262" class="blob-num js-line-number" data-line-number="262"></td>
        <td id="LC262" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">get_specific_subnodes</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>, <span class="pl-smi">name</span>, <span class="pl-smi">recursive</span><span class="pl-k">=</span><span class="pl-c1">0</span>):</td>
      </tr>
      <tr>
        <td id="L263" class="blob-num js-line-number" data-line-number="263"></td>
        <td id="LC263" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Given a node and a name, return a list of child `ELEMENT_NODEs`, that</span></td>
      </tr>
      <tr>
        <td id="L264" class="blob-num js-line-number" data-line-number="264"></td>
        <td id="LC264" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        have a `tagName` matching the `name`. Search recursively for `recursive`</span></td>
      </tr>
      <tr>
        <td id="L265" class="blob-num js-line-number" data-line-number="265"></td>
        <td id="LC265" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        levels.</span></td>
      </tr>
      <tr>
        <td id="L266" class="blob-num js-line-number" data-line-number="266"></td>
        <td id="LC266" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L267" class="blob-num js-line-number" data-line-number="267"></td>
        <td id="LC267" class="blob-code blob-code-inner js-file-line">        children <span class="pl-k">=</span> [x <span class="pl-k">for</span> x <span class="pl-k">in</span> node.childNodes <span class="pl-k">if</span> x.nodeType <span class="pl-k">==</span> x.ELEMENT_NODE]</td>
      </tr>
      <tr>
        <td id="L268" class="blob-num js-line-number" data-line-number="268"></td>
        <td id="LC268" class="blob-code blob-code-inner js-file-line">        ret <span class="pl-k">=</span> [x <span class="pl-k">for</span> x <span class="pl-k">in</span> children <span class="pl-k">if</span> x.tagName <span class="pl-k">==</span> name]</td>
      </tr>
      <tr>
        <td id="L269" class="blob-num js-line-number" data-line-number="269"></td>
        <td id="LC269" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> recursive <span class="pl-k">&gt;</span> <span class="pl-c1">0</span>:</td>
      </tr>
      <tr>
        <td id="L270" class="blob-num js-line-number" data-line-number="270"></td>
        <td id="LC270" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> x <span class="pl-k">in</span> children:</td>
      </tr>
      <tr>
        <td id="L271" class="blob-num js-line-number" data-line-number="271"></td>
        <td id="LC271" class="blob-code blob-code-inner js-file-line">                ret.extend(<span class="pl-v">self</span>.get_specific_subnodes(x, name, recursive<span class="pl-k">-</span><span class="pl-c1">1</span>))</td>
      </tr>
      <tr>
        <td id="L272" class="blob-num js-line-number" data-line-number="272"></td>
        <td id="LC272" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> ret</td>
      </tr>
      <tr>
        <td id="L273" class="blob-num js-line-number" data-line-number="273"></td>
        <td id="LC273" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L274" class="blob-num js-line-number" data-line-number="274"></td>
        <td id="LC274" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">get_specific_nodes</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>, <span class="pl-smi">names</span>):</td>
      </tr>
      <tr>
        <td id="L275" class="blob-num js-line-number" data-line-number="275"></td>
        <td id="LC275" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Given a node and a sequence of strings in `names`, return a</span></td>
      </tr>
      <tr>
        <td id="L276" class="blob-num js-line-number" data-line-number="276"></td>
        <td id="LC276" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        dictionary containing the names as keys and child</span></td>
      </tr>
      <tr>
        <td id="L277" class="blob-num js-line-number" data-line-number="277"></td>
        <td id="LC277" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        `ELEMENT_NODEs`, that have a `tagName` equal to the name.</span></td>
      </tr>
      <tr>
        <td id="L278" class="blob-num js-line-number" data-line-number="278"></td>
        <td id="LC278" class="blob-code blob-code-inner js-file-line"><span class="pl-s"></span></td>
      </tr>
      <tr>
        <td id="L279" class="blob-num js-line-number" data-line-number="279"></td>
        <td id="LC279" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L280" class="blob-num js-line-number" data-line-number="280"></td>
        <td id="LC280" class="blob-code blob-code-inner js-file-line">        nodes <span class="pl-k">=</span> [(x.tagName, x) <span class="pl-k">for</span> x <span class="pl-k">in</span> node.childNodes</td>
      </tr>
      <tr>
        <td id="L281" class="blob-num js-line-number" data-line-number="281"></td>
        <td id="LC281" class="blob-code blob-code-inner js-file-line">                 <span class="pl-k">if</span> x.nodeType <span class="pl-k">==</span> x.ELEMENT_NODE <span class="pl-k">and</span></td>
      </tr>
      <tr>
        <td id="L282" class="blob-num js-line-number" data-line-number="282"></td>
        <td id="LC282" class="blob-code blob-code-inner js-file-line">                 x.tagName <span class="pl-k">in</span> names]</td>
      </tr>
      <tr>
        <td id="L283" class="blob-num js-line-number" data-line-number="283"></td>
        <td id="LC283" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> <span class="pl-c1">dict</span>(nodes)</td>
      </tr>
      <tr>
        <td id="L284" class="blob-num js-line-number" data-line-number="284"></td>
        <td id="LC284" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L285" class="blob-num js-line-number" data-line-number="285"></td>
        <td id="LC285" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">add_text</span>(<span class="pl-smi">self</span>, <span class="pl-smi">value</span>):</td>
      </tr>
      <tr>
        <td id="L286" class="blob-num js-line-number" data-line-number="286"></td>
        <td id="LC286" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Adds text corresponding to `value` into `self.pieces`.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L287" class="blob-num js-line-number" data-line-number="287"></td>
        <td id="LC287" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">isinstance</span>(value, (<span class="pl-c1">list</span>, <span class="pl-c1">tuple</span>)):</td>
      </tr>
      <tr>
        <td id="L288" class="blob-num js-line-number" data-line-number="288"></td>
        <td id="LC288" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.extend(value)</td>
      </tr>
      <tr>
        <td id="L289" class="blob-num js-line-number" data-line-number="289"></td>
        <td id="LC289" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L290" class="blob-num js-line-number" data-line-number="290"></td>
        <td id="LC290" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.append(value)</td>
      </tr>
      <tr>
        <td id="L291" class="blob-num js-line-number" data-line-number="291"></td>
        <td id="LC291" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L292" class="blob-num js-line-number" data-line-number="292"></td>
        <td id="LC292" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">start_new_paragraph</span>(<span class="pl-smi">self</span>):</td>
      </tr>
      <tr>
        <td id="L293" class="blob-num js-line-number" data-line-number="293"></td>
        <td id="LC293" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Make sure to create an empty line. This is overridden, if the previous</span></td>
      </tr>
      <tr>
        <td id="L294" class="blob-num js-line-number" data-line-number="294"></td>
        <td id="LC294" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        text ends with the special marker &#39;&#39;. In that case, nothing is done.</span></td>
      </tr>
      <tr>
        <td id="L295" class="blob-num js-line-number" data-line-number="295"></td>
        <td id="LC295" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L296" class="blob-num js-line-number" data-line-number="296"></td>
        <td id="LC296" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">==</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]: <span class="pl-c"># respect special marker</span></td>
      </tr>
      <tr>
        <td id="L297" class="blob-num js-line-number" data-line-number="297"></td>
        <td id="LC297" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L298" class="blob-num js-line-number" data-line-number="298"></td>
        <td id="LC298" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> <span class="pl-v">self</span>.pieces <span class="pl-k">==</span> []: <span class="pl-c"># first paragraph, add &#39;\n&#39;, override with &#39;&#39;</span></td>
      </tr>
      <tr>
        <td id="L299" class="blob-num js-line-number" data-line-number="299"></td>
        <td id="LC299" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L300" class="blob-num js-line-number" data-line-number="300"></td>
        <td id="LC300" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>][<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>: <span class="pl-c"># previous line not ended</span></td>
      </tr>
      <tr>
        <td id="L301" class="blob-num js-line-number" data-line-number="301"></td>
        <td id="LC301" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.extend([<span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span> ,<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L302" class="blob-num js-line-number" data-line-number="302"></td>
        <td id="LC302" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>: <span class="pl-c">#default</span></td>
      </tr>
      <tr>
        <td id="L303" class="blob-num js-line-number" data-line-number="303"></td>
        <td id="LC303" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.append(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L304" class="blob-num js-line-number" data-line-number="304"></td>
        <td id="LC304" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L305" class="blob-num js-line-number" data-line-number="305"></td>
        <td id="LC305" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">add_line_with_subsequent_indent</span>(<span class="pl-smi">self</span>, <span class="pl-smi">line</span>, <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>):</td>
      </tr>
      <tr>
        <td id="L306" class="blob-num js-line-number" data-line-number="306"></td>
        <td id="LC306" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Add line of text and wrap such that subsequent lines are indented</span></td>
      </tr>
      <tr>
        <td id="L307" class="blob-num js-line-number" data-line-number="307"></td>
        <td id="LC307" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        by `indent` spaces.</span></td>
      </tr>
      <tr>
        <td id="L308" class="blob-num js-line-number" data-line-number="308"></td>
        <td id="LC308" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L309" class="blob-num js-line-number" data-line-number="309"></td>
        <td id="LC309" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">isinstance</span>(line, (<span class="pl-c1">list</span>, <span class="pl-c1">tuple</span>)):</td>
      </tr>
      <tr>
        <td id="L310" class="blob-num js-line-number" data-line-number="310"></td>
        <td id="LC310" class="blob-code blob-code-inner js-file-line">            line <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(line)</td>
      </tr>
      <tr>
        <td id="L311" class="blob-num js-line-number" data-line-number="311"></td>
        <td id="LC311" class="blob-code blob-code-inner js-file-line">        line <span class="pl-k">=</span> line.strip()</td>
      </tr>
      <tr>
        <td id="L312" class="blob-num js-line-number" data-line-number="312"></td>
        <td id="LC312" class="blob-code blob-code-inner js-file-line">        width <span class="pl-k">=</span> <span class="pl-v">self</span>.textwidth<span class="pl-k">-</span><span class="pl-v">self</span>.indent<span class="pl-k">-</span>indent</td>
      </tr>
      <tr>
        <td id="L313" class="blob-num js-line-number" data-line-number="313"></td>
        <td id="LC313" class="blob-code blob-code-inner js-file-line">        wrapped_lines <span class="pl-k">=</span> textwrap.wrap(line[indent:], <span class="pl-smi">width</span><span class="pl-k">=</span>width)</td>
      </tr>
      <tr>
        <td id="L314" class="blob-num js-line-number" data-line-number="314"></td>
        <td id="LC314" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> i <span class="pl-k">in</span> <span class="pl-c1">range</span>(<span class="pl-c1">len</span>(wrapped_lines)):</td>
      </tr>
      <tr>
        <td id="L315" class="blob-num js-line-number" data-line-number="315"></td>
        <td id="LC315" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> wrapped_lines[i] <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L316" class="blob-num js-line-number" data-line-number="316"></td>
        <td id="LC316" class="blob-code blob-code-inner js-file-line">                wrapped_lines[i] <span class="pl-k">=</span> indent <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> wrapped_lines[i]</td>
      </tr>
      <tr>
        <td id="L317" class="blob-num js-line-number" data-line-number="317"></td>
        <td id="LC317" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces.append(line[:indent] <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>.join(wrapped_lines)[indent:] <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L318" class="blob-num js-line-number" data-line-number="318"></td>
        <td id="LC318" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L319" class="blob-num js-line-number" data-line-number="319"></td>
        <td id="LC319" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">extract_text</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L320" class="blob-num js-line-number" data-line-number="320"></td>
        <td id="LC320" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Return the string representation of the node or list of nodes by parsing the</span></td>
      </tr>
      <tr>
        <td id="L321" class="blob-num js-line-number" data-line-number="321"></td>
        <td id="LC321" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        subnodes, but returning the result as a string instead of adding it to `self.pieces`.</span></td>
      </tr>
      <tr>
        <td id="L322" class="blob-num js-line-number" data-line-number="322"></td>
        <td id="LC322" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        Note that this allows extracting text even if the node is in the ignore list.</span></td>
      </tr>
      <tr>
        <td id="L323" class="blob-num js-line-number" data-line-number="323"></td>
        <td id="LC323" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L324" class="blob-num js-line-number" data-line-number="324"></td>
        <td id="LC324" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-k">not</span> <span class="pl-c1">isinstance</span>(node, (<span class="pl-c1">list</span>, <span class="pl-c1">tuple</span>)):</td>
      </tr>
      <tr>
        <td id="L325" class="blob-num js-line-number" data-line-number="325"></td>
        <td id="LC325" class="blob-code blob-code-inner js-file-line">            node <span class="pl-k">=</span> [node]</td>
      </tr>
      <tr>
        <td id="L326" class="blob-num js-line-number" data-line-number="326"></td>
        <td id="LC326" class="blob-code blob-code-inner js-file-line">        pieces, <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces, [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L327" class="blob-num js-line-number" data-line-number="327"></td>
        <td id="LC327" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> node:</td>
      </tr>
      <tr>
        <td id="L328" class="blob-num js-line-number" data-line-number="328"></td>
        <td id="LC328" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> sn <span class="pl-k">in</span> n.childNodes:</td>
      </tr>
      <tr>
        <td id="L329" class="blob-num js-line-number" data-line-number="329"></td>
        <td id="LC329" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.parse(sn)</td>
      </tr>
      <tr>
        <td id="L330" class="blob-num js-line-number" data-line-number="330"></td>
        <td id="LC330" class="blob-code blob-code-inner js-file-line">        ret <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces)</td>
      </tr>
      <tr>
        <td id="L331" class="blob-num js-line-number" data-line-number="331"></td>
        <td id="LC331" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> pieces</td>
      </tr>
      <tr>
        <td id="L332" class="blob-num js-line-number" data-line-number="332"></td>
        <td id="LC332" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> ret</td>
      </tr>
      <tr>
        <td id="L333" class="blob-num js-line-number" data-line-number="333"></td>
        <td id="LC333" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L334" class="blob-num js-line-number" data-line-number="334"></td>
        <td id="LC334" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">get_function_signature</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L335" class="blob-num js-line-number" data-line-number="335"></td>
        <td id="LC335" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Returns the function signature string for memberdef nodes.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L336" class="blob-num js-line-number" data-line-number="336"></td>
        <td id="LC336" class="blob-code blob-code-inner js-file-line">        name <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L337" class="blob-num js-line-number" data-line-number="337"></td>
        <td id="LC337" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.with_type_info:</td>
      </tr>
      <tr>
        <td id="L338" class="blob-num js-line-number" data-line-number="338"></td>
        <td id="LC338" class="blob-code blob-code-inner js-file-line">            argsstring <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>argsstring<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L339" class="blob-num js-line-number" data-line-number="339"></td>
        <td id="LC339" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L340" class="blob-num js-line-number" data-line-number="340"></td>
        <td id="LC340" class="blob-code blob-code-inner js-file-line">            argsstring <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L341" class="blob-num js-line-number" data-line-number="341"></td>
        <td id="LC341" class="blob-code blob-code-inner js-file-line">            param_id <span class="pl-k">=</span> <span class="pl-c1">1</span></td>
      </tr>
      <tr>
        <td id="L342" class="blob-num js-line-number" data-line-number="342"></td>
        <td id="LC342" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> n_param <span class="pl-k">in</span> <span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>param<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L343" class="blob-num js-line-number" data-line-number="343"></td>
        <td id="LC343" class="blob-code blob-code-inner js-file-line">                declname <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n_param, <span class="pl-s"><span class="pl-pds">&#39;</span>declname<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L344" class="blob-num js-line-number" data-line-number="344"></td>
        <td id="LC344" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">if</span> <span class="pl-k">not</span> declname:</td>
      </tr>
      <tr>
        <td id="L345" class="blob-num js-line-number" data-line-number="345"></td>
        <td id="LC345" class="blob-code blob-code-inner js-file-line">                    declname <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>arg<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-c1">str</span>(param_id)</td>
      </tr>
      <tr>
        <td id="L346" class="blob-num js-line-number" data-line-number="346"></td>
        <td id="LC346" class="blob-code blob-code-inner js-file-line">                defval <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n_param, <span class="pl-s"><span class="pl-pds">&#39;</span>defval<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L347" class="blob-num js-line-number" data-line-number="347"></td>
        <td id="LC347" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">if</span> defval:</td>
      </tr>
      <tr>
        <td id="L348" class="blob-num js-line-number" data-line-number="348"></td>
        <td id="LC348" class="blob-code blob-code-inner js-file-line">                    defval <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>=<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> defval</td>
      </tr>
      <tr>
        <td id="L349" class="blob-num js-line-number" data-line-number="349"></td>
        <td id="LC349" class="blob-code blob-code-inner js-file-line">                argsstring.append(declname <span class="pl-k">+</span> defval)</td>
      </tr>
      <tr>
        <td id="L350" class="blob-num js-line-number" data-line-number="350"></td>
        <td id="LC350" class="blob-code blob-code-inner js-file-line">                param_id <span class="pl-k">=</span> param_id <span class="pl-k">+</span> <span class="pl-c1">1</span></td>
      </tr>
      <tr>
        <td id="L351" class="blob-num js-line-number" data-line-number="351"></td>
        <td id="LC351" class="blob-code blob-code-inner js-file-line">            argsstring <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>(<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>, <span class="pl-pds">&#39;</span></span>.join(argsstring) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>)<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L352" class="blob-num js-line-number" data-line-number="352"></td>
        <td id="LC352" class="blob-code blob-code-inner js-file-line">        <span class="pl-c1">type</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>type<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L353" class="blob-num js-line-number" data-line-number="353"></td>
        <td id="LC353" class="blob-code blob-code-inner js-file-line">        function_definition <span class="pl-k">=</span> name <span class="pl-k">+</span> argsstring</td>
      </tr>
      <tr>
        <td id="L354" class="blob-num js-line-number" data-line-number="354"></td>
        <td id="LC354" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">type</span> <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span> <span class="pl-k">and</span> <span class="pl-c1">type</span> <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>void<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L355" class="blob-num js-line-number" data-line-number="355"></td>
        <td id="LC355" class="blob-code blob-code-inner js-file-line">            function_definition <span class="pl-k">=</span> function_definition <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span> -&gt; <span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-c1">type</span></td>
      </tr>
      <tr>
        <td id="L356" class="blob-num js-line-number" data-line-number="356"></td>
        <td id="LC356" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> function_definition <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>`  <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L357" class="blob-num js-line-number" data-line-number="357"></td>
        <td id="LC357" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L358" class="blob-num js-line-number" data-line-number="358"></td>
        <td id="LC358" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Special parsing tasks (need to be called manually)</span></td>
      </tr>
      <tr>
        <td id="L359" class="blob-num js-line-number" data-line-number="359"></td>
        <td id="LC359" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">make_constructor_list</span>(<span class="pl-smi">self</span>, <span class="pl-smi">constructor_nodes</span>, <span class="pl-smi">classname</span>):</td>
      </tr>
      <tr>
        <td id="L360" class="blob-num js-line-number" data-line-number="360"></td>
        <td id="LC360" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Produces the &quot;Constructors&quot; section and the constructor signatures</span></td>
      </tr>
      <tr>
        <td id="L361" class="blob-num js-line-number" data-line-number="361"></td>
        <td id="LC361" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        (since swig does not do so for classes) for class docstrings.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L362" class="blob-num js-line-number" data-line-number="362"></td>
        <td id="LC362" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> constructor_nodes <span class="pl-k">==</span> []:</td>
      </tr>
      <tr>
        <td id="L363" class="blob-num js-line-number" data-line-number="363"></td>
        <td id="LC363" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L364" class="blob-num js-line-number" data-line-number="364"></td>
        <td id="LC364" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>Constructors<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L365" class="blob-num js-line-number" data-line-number="365"></td>
        <td id="LC365" class="blob-code blob-code-inner js-file-line">                       <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>------------<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L366" class="blob-num js-line-number" data-line-number="366"></td>
        <td id="LC366" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> constructor_nodes:</td>
      </tr>
      <tr>
        <td id="L367" class="blob-num js-line-number" data-line-number="367"></td>
        <td id="LC367" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L368" class="blob-num js-line-number" data-line-number="368"></td>
        <td id="LC368" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_line_with_subsequent_indent(<span class="pl-s"><span class="pl-pds">&#39;</span>* <span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-v">self</span>.get_function_signature(n))</td>
      </tr>
      <tr>
        <td id="L369" class="blob-num js-line-number" data-line-number="369"></td>
        <td id="LC369" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(n, <span class="pl-smi">pieces</span> <span class="pl-k">=</span> [], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>, <span class="pl-smi">ignore</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L370" class="blob-num js-line-number" data-line-number="370"></td>
        <td id="LC370" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L371" class="blob-num js-line-number" data-line-number="371"></td>
        <td id="LC371" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">make_attribute_list</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L372" class="blob-num js-line-number" data-line-number="372"></td>
        <td id="LC372" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Produces the &quot;Attributes&quot; section in class docstrings for public</span></td>
      </tr>
      <tr>
        <td id="L373" class="blob-num js-line-number" data-line-number="373"></td>
        <td id="LC373" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        member variables (attributes).</span></td>
      </tr>
      <tr>
        <td id="L374" class="blob-num js-line-number" data-line-number="374"></td>
        <td id="LC374" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L375" class="blob-num js-line-number" data-line-number="375"></td>
        <td id="LC375" class="blob-code blob-code-inner js-file-line">        atr_nodes <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L376" class="blob-num js-line-number" data-line-number="376"></td>
        <td id="LC376" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> <span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>memberdef<span class="pl-pds">&#39;</span></span>, <span class="pl-smi">recursive</span><span class="pl-k">=</span><span class="pl-c1">2</span>):</td>
      </tr>
      <tr>
        <td id="L377" class="blob-num js-line-number" data-line-number="377"></td>
        <td id="LC377" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> n.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>variable<span class="pl-pds">&#39;</span></span> <span class="pl-k">and</span> n.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>prot<span class="pl-pds">&#39;</span></span>].value <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>public<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L378" class="blob-num js-line-number" data-line-number="378"></td>
        <td id="LC378" class="blob-code blob-code-inner js-file-line">                atr_nodes.append(n)</td>
      </tr>
      <tr>
        <td id="L379" class="blob-num js-line-number" data-line-number="379"></td>
        <td id="LC379" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-k">not</span> atr_nodes:</td>
      </tr>
      <tr>
        <td id="L380" class="blob-num js-line-number" data-line-number="380"></td>
        <td id="LC380" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L381" class="blob-num js-line-number" data-line-number="381"></td>
        <td id="LC381" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>Attributes<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L382" class="blob-num js-line-number" data-line-number="382"></td>
        <td id="LC382" class="blob-code blob-code-inner js-file-line">                       <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>----------<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L383" class="blob-num js-line-number" data-line-number="383"></td>
        <td id="LC383" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> atr_nodes:</td>
      </tr>
      <tr>
        <td id="L384" class="blob-num js-line-number" data-line-number="384"></td>
        <td id="LC384" class="blob-code blob-code-inner js-file-line">            name <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L385" class="blob-num js-line-number" data-line-number="385"></td>
        <td id="LC385" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>* <span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>, name, <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span> : <span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L386" class="blob-num js-line-number" data-line-number="386"></td>
        <td id="LC386" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>, <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n, <span class="pl-s"><span class="pl-pds">&#39;</span>type<span class="pl-pds">&#39;</span></span>)), <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L387" class="blob-num js-line-number" data-line-number="387"></td>
        <td id="LC387" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L388" class="blob-num js-line-number" data-line-number="388"></td>
        <td id="LC388" class="blob-code blob-code-inner js-file-line">            restrict <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>briefdescription<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>detaileddescription<span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L389" class="blob-num js-line-number" data-line-number="389"></td>
        <td id="LC389" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(n, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>, <span class="pl-smi">restrict</span><span class="pl-k">=</span>restrict)</td>
      </tr>
      <tr>
        <td id="L390" class="blob-num js-line-number" data-line-number="390"></td>
        <td id="LC390" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L391" class="blob-num js-line-number" data-line-number="391"></td>
        <td id="LC391" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">get_memberdef_nodes_and_signatures</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>, <span class="pl-smi">kind</span>):</td>
      </tr>
      <tr>
        <td id="L392" class="blob-num js-line-number" data-line-number="392"></td>
        <td id="LC392" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Collects the memberdef nodes and corresponding signatures that</span></td>
      </tr>
      <tr>
        <td id="L393" class="blob-num js-line-number" data-line-number="393"></td>
        <td id="LC393" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        correspond to public function entries that are at most depth 2 deeper</span></td>
      </tr>
      <tr>
        <td id="L394" class="blob-num js-line-number" data-line-number="394"></td>
        <td id="LC394" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        than the current (compounddef) node. Returns a dictionary with </span></td>
      </tr>
      <tr>
        <td id="L395" class="blob-num js-line-number" data-line-number="395"></td>
        <td id="LC395" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        function signatures (what swig expects after the <span class="pl-c1">%f</span>eature directive)</span></td>
      </tr>
      <tr>
        <td id="L396" class="blob-num js-line-number" data-line-number="396"></td>
        <td id="LC396" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        as keys, and a list of corresponding memberdef nodes as values.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L397" class="blob-num js-line-number" data-line-number="397"></td>
        <td id="LC397" class="blob-code blob-code-inner js-file-line">        sig_dict <span class="pl-k">=</span> {}</td>
      </tr>
      <tr>
        <td id="L398" class="blob-num js-line-number" data-line-number="398"></td>
        <td id="LC398" class="blob-code blob-code-inner js-file-line">        sig_prefix <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L399" class="blob-num js-line-number" data-line-number="399"></td>
        <td id="LC399" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>file<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L400" class="blob-num js-line-number" data-line-number="400"></td>
        <td id="LC400" class="blob-code blob-code-inner js-file-line">            ns_node <span class="pl-k">=</span> node.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>innernamespace<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L401" class="blob-num js-line-number" data-line-number="401"></td>
        <td id="LC401" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-k">not</span> ns_node <span class="pl-k">and</span> kind <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L402" class="blob-num js-line-number" data-line-number="402"></td>
        <td id="LC402" class="blob-code blob-code-inner js-file-line">                ns_node <span class="pl-k">=</span> node.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>compoundname<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L403" class="blob-num js-line-number" data-line-number="403"></td>
        <td id="LC403" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> ns_node:</td>
      </tr>
      <tr>
        <td id="L404" class="blob-num js-line-number" data-line-number="404"></td>
        <td id="LC404" class="blob-code blob-code-inner js-file-line">                sig_prefix <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(ns_node[<span class="pl-c1">0</span>]) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>::<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L405" class="blob-num js-line-number" data-line-number="405"></td>
        <td id="LC405" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>class<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>struct<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L406" class="blob-num js-line-number" data-line-number="406"></td>
        <td id="LC406" class="blob-code blob-code-inner js-file-line">            <span class="pl-c"># Get the full function name.</span></td>
      </tr>
      <tr>
        <td id="L407" class="blob-num js-line-number" data-line-number="407"></td>
        <td id="LC407" class="blob-code blob-code-inner js-file-line">            cn_node <span class="pl-k">=</span> node.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>compoundname<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L408" class="blob-num js-line-number" data-line-number="408"></td>
        <td id="LC408" class="blob-code blob-code-inner js-file-line">            sig_prefix <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(cn_node[<span class="pl-c1">0</span>]) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>::<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L409" class="blob-num js-line-number" data-line-number="409"></td>
        <td id="LC409" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L410" class="blob-num js-line-number" data-line-number="410"></td>
        <td id="LC410" class="blob-code blob-code-inner js-file-line">        md_nodes <span class="pl-k">=</span> <span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>memberdef<span class="pl-pds">&#39;</span></span>, <span class="pl-smi">recursive</span><span class="pl-k">=</span><span class="pl-c1">2</span>)</td>
      </tr>
      <tr>
        <td id="L411" class="blob-num js-line-number" data-line-number="411"></td>
        <td id="LC411" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> md_nodes:</td>
      </tr>
      <tr>
        <td id="L412" class="blob-num js-line-number" data-line-number="412"></td>
        <td id="LC412" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> n.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>prot<span class="pl-pds">&#39;</span></span>].value <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>public<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L413" class="blob-num js-line-number" data-line-number="413"></td>
        <td id="LC413" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">continue</span></td>
      </tr>
      <tr>
        <td id="L414" class="blob-num js-line-number" data-line-number="414"></td>
        <td id="LC414" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> n.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value <span class="pl-k">in</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>variable<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>typedef<span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L415" class="blob-num js-line-number" data-line-number="415"></td>
        <td id="LC415" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">continue</span></td>
      </tr>
      <tr>
        <td id="L416" class="blob-num js-line-number" data-line-number="416"></td>
        <td id="LC416" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-k">not</span> <span class="pl-v">self</span>.get_specific_subnodes(n, <span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L417" class="blob-num js-line-number" data-line-number="417"></td>
        <td id="LC417" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">continue</span></td>
      </tr>
      <tr>
        <td id="L418" class="blob-num js-line-number" data-line-number="418"></td>
        <td id="LC418" class="blob-code blob-code-inner js-file-line">            name <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L419" class="blob-num js-line-number" data-line-number="419"></td>
        <td id="LC419" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> name[:<span class="pl-c1">8</span>] <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>operator<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L420" class="blob-num js-line-number" data-line-number="420"></td>
        <td id="LC420" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">continue</span></td>
      </tr>
      <tr>
        <td id="L421" class="blob-num js-line-number" data-line-number="421"></td>
        <td id="LC421" class="blob-code blob-code-inner js-file-line">            sig <span class="pl-k">=</span> sig_prefix <span class="pl-k">+</span> name</td>
      </tr>
      <tr>
        <td id="L422" class="blob-num js-line-number" data-line-number="422"></td>
        <td id="LC422" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> sig <span class="pl-k">in</span> sig_dict:</td>
      </tr>
      <tr>
        <td id="L423" class="blob-num js-line-number" data-line-number="423"></td>
        <td id="LC423" class="blob-code blob-code-inner js-file-line">                sig_dict[sig].append(n)</td>
      </tr>
      <tr>
        <td id="L424" class="blob-num js-line-number" data-line-number="424"></td>
        <td id="LC424" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L425" class="blob-num js-line-number" data-line-number="425"></td>
        <td id="LC425" class="blob-code blob-code-inner js-file-line">                sig_dict[sig] <span class="pl-k">=</span> [n]</td>
      </tr>
      <tr>
        <td id="L426" class="blob-num js-line-number" data-line-number="426"></td>
        <td id="LC426" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">return</span> sig_dict</td>
      </tr>
      <tr>
        <td id="L427" class="blob-num js-line-number" data-line-number="427"></td>
        <td id="LC427" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L428" class="blob-num js-line-number" data-line-number="428"></td>
        <td id="LC428" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">handle_typical_memberdefs_no_overload</span>(<span class="pl-smi">self</span>, <span class="pl-smi">signature</span>, <span class="pl-smi">memberdef_nodes</span>):</td>
      </tr>
      <tr>
        <td id="L429" class="blob-num js-line-number" data-line-number="429"></td>
        <td id="LC429" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Produce standard documentation for memberdef_nodes.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L430" class="blob-num js-line-number" data-line-number="430"></td>
        <td id="LC430" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> memberdef_nodes:</td>
      </tr>
      <tr>
        <td id="L431" class="blob-num js-line-number" data-line-number="431"></td>
        <td id="LC431" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-c1">%f</span>eature(&quot;docstring&quot;) <span class="pl-pds">&#39;</span></span>, signature, <span class="pl-s"><span class="pl-pds">&#39;</span> &quot;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L432" class="blob-num js-line-number" data-line-number="432"></td>
        <td id="LC432" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.with_function_signature:</td>
      </tr>
      <tr>
        <td id="L433" class="blob-num js-line-number" data-line-number="433"></td>
        <td id="LC433" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.add_line_with_subsequent_indent(<span class="pl-v">self</span>.get_function_signature(n))</td>
      </tr>
      <tr>
        <td id="L434" class="blob-num js-line-number" data-line-number="434"></td>
        <td id="LC434" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(n, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[], <span class="pl-smi">ignore</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L435" class="blob-num js-line-number" data-line-number="435"></td>
        <td id="LC435" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>&quot;;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L436" class="blob-num js-line-number" data-line-number="436"></td>
        <td id="LC436" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L437" class="blob-num js-line-number" data-line-number="437"></td>
        <td id="LC437" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">handle_typical_memberdefs</span>(<span class="pl-smi">self</span>, <span class="pl-smi">signature</span>, <span class="pl-smi">memberdef_nodes</span>):</td>
      </tr>
      <tr>
        <td id="L438" class="blob-num js-line-number" data-line-number="438"></td>
        <td id="LC438" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Produces docstring entries containing an &quot;Overloaded function&quot;</span></td>
      </tr>
      <tr>
        <td id="L439" class="blob-num js-line-number" data-line-number="439"></td>
        <td id="LC439" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        section with the documentation for each overload, if the function is</span></td>
      </tr>
      <tr>
        <td id="L440" class="blob-num js-line-number" data-line-number="440"></td>
        <td id="LC440" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        overloaded and self.with_overloaded_functions is set. Else, produce</span></td>
      </tr>
      <tr>
        <td id="L441" class="blob-num js-line-number" data-line-number="441"></td>
        <td id="LC441" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        normal documentation.</span></td>
      </tr>
      <tr>
        <td id="L442" class="blob-num js-line-number" data-line-number="442"></td>
        <td id="LC442" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L443" class="blob-num js-line-number" data-line-number="443"></td>
        <td id="LC443" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">len</span>(memberdef_nodes) <span class="pl-k">==</span> <span class="pl-c1">1</span> <span class="pl-k">or</span> <span class="pl-k">not</span> <span class="pl-v">self</span>.with_overloaded_functions:</td>
      </tr>
      <tr>
        <td id="L444" class="blob-num js-line-number" data-line-number="444"></td>
        <td id="LC444" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.handle_typical_memberdefs_no_overload(signature, memberdef_nodes)</td>
      </tr>
      <tr>
        <td id="L445" class="blob-num js-line-number" data-line-number="445"></td>
        <td id="LC445" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L446" class="blob-num js-line-number" data-line-number="446"></td>
        <td id="LC446" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L447" class="blob-num js-line-number" data-line-number="447"></td>
        <td id="LC447" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-c1">%f</span>eature(&quot;docstring&quot;) <span class="pl-pds">&#39;</span></span>, signature, <span class="pl-s"><span class="pl-pds">&#39;</span> &quot;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L448" class="blob-num js-line-number" data-line-number="448"></td>
        <td id="LC448" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.with_function_signature:</td>
      </tr>
      <tr>
        <td id="L449" class="blob-num js-line-number" data-line-number="449"></td>
        <td id="LC449" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> n <span class="pl-k">in</span> memberdef_nodes:</td>
      </tr>
      <tr>
        <td id="L450" class="blob-num js-line-number" data-line-number="450"></td>
        <td id="LC450" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.add_line_with_subsequent_indent(<span class="pl-v">self</span>.get_function_signature(n))</td>
      </tr>
      <tr>
        <td id="L451" class="blob-num js-line-number" data-line-number="451"></td>
        <td id="LC451" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L452" class="blob-num js-line-number" data-line-number="452"></td>
        <td id="LC452" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>Overloaded function<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L453" class="blob-num js-line-number" data-line-number="453"></td>
        <td id="LC453" class="blob-code blob-code-inner js-file-line">                       <span class="pl-s"><span class="pl-pds">&#39;</span>-------------------<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L454" class="blob-num js-line-number" data-line-number="454"></td>
        <td id="LC454" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> memberdef_nodes:</td>
      </tr>
      <tr>
        <td id="L455" class="blob-num js-line-number" data-line-number="455"></td>
        <td id="LC455" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L456" class="blob-num js-line-number" data-line-number="456"></td>
        <td id="LC456" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_line_with_subsequent_indent(<span class="pl-s"><span class="pl-pds">&#39;</span>* <span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-v">self</span>.get_function_signature(n))</td>
      </tr>
      <tr>
        <td id="L457" class="blob-num js-line-number" data-line-number="457"></td>
        <td id="LC457" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(n, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>, <span class="pl-smi">ignore</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L458" class="blob-num js-line-number" data-line-number="458"></td>
        <td id="LC458" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>&quot;;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L459" class="blob-num js-line-number" data-line-number="459"></td>
        <td id="LC459" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L460" class="blob-num js-line-number" data-line-number="460"></td>
        <td id="LC460" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L461" class="blob-num js-line-number" data-line-number="461"></td>
        <td id="LC461" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Tag handlers</span></td>
      </tr>
      <tr>
        <td id="L462" class="blob-num js-line-number" data-line-number="462"></td>
        <td id="LC462" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_linebreak</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L463" class="blob-num js-line-number" data-line-number="463"></td>
        <td id="LC463" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L464" class="blob-num js-line-number" data-line-number="464"></td>
        <td id="LC464" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L465" class="blob-num js-line-number" data-line-number="465"></td>
        <td id="LC465" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_ndash</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L466" class="blob-num js-line-number" data-line-number="466"></td>
        <td id="LC466" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>--<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L467" class="blob-num js-line-number" data-line-number="467"></td>
        <td id="LC467" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L468" class="blob-num js-line-number" data-line-number="468"></td>
        <td id="LC468" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_mdash</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L469" class="blob-num js-line-number" data-line-number="469"></td>
        <td id="LC469" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>---<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L470" class="blob-num js-line-number" data-line-number="470"></td>
        <td id="LC470" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L471" class="blob-num js-line-number" data-line-number="471"></td>
        <td id="LC471" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_emphasis</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L472" class="blob-num js-line-number" data-line-number="472"></td>
        <td id="LC472" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.surround_parse(node, <span class="pl-s"><span class="pl-pds">&#39;</span>*<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>*<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L473" class="blob-num js-line-number" data-line-number="473"></td>
        <td id="LC473" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L474" class="blob-num js-line-number" data-line-number="474"></td>
        <td id="LC474" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_bold</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L475" class="blob-num js-line-number" data-line-number="475"></td>
        <td id="LC475" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.surround_parse(node, <span class="pl-s"><span class="pl-pds">&#39;</span>**<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>**<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L476" class="blob-num js-line-number" data-line-number="476"></td>
        <td id="LC476" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L477" class="blob-num js-line-number" data-line-number="477"></td>
        <td id="LC477" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_computeroutput</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L478" class="blob-num js-line-number" data-line-number="478"></td>
        <td id="LC478" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.surround_parse(node, <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L479" class="blob-num js-line-number" data-line-number="479"></td>
        <td id="LC479" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L480" class="blob-num js-line-number" data-line-number="480"></td>
        <td id="LC480" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_heading</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L481" class="blob-num js-line-number" data-line-number="481"></td>
        <td id="LC481" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L482" class="blob-num js-line-number" data-line-number="482"></td>
        <td id="LC482" class="blob-code blob-code-inner js-file-line">        pieces, <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces, [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L483" class="blob-num js-line-number" data-line-number="483"></td>
        <td id="LC483" class="blob-code blob-code-inner js-file-line">        level <span class="pl-k">=</span> <span class="pl-c1">int</span>(node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>level<span class="pl-pds">&#39;</span></span>].value)</td>
      </tr>
      <tr>
        <td id="L484" class="blob-num js-line-number" data-line-number="484"></td>
        <td id="LC484" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L485" class="blob-num js-line-number" data-line-number="485"></td>
        <td id="LC485" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> level <span class="pl-k">==</span> <span class="pl-c1">1</span>:</td>
      </tr>
      <tr>
        <td id="L486" class="blob-num js-line-number" data-line-number="486"></td>
        <td id="LC486" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.insert(<span class="pl-c1">0</span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L487" class="blob-num js-line-number" data-line-number="487"></td>
        <td id="LC487" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-c1">len</span>(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces).strip()) <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span>=<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L488" class="blob-num js-line-number" data-line-number="488"></td>
        <td id="LC488" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> level <span class="pl-k">==</span> <span class="pl-c1">2</span>:</td>
      </tr>
      <tr>
        <td id="L489" class="blob-num js-line-number" data-line-number="489"></td>
        <td id="LC489" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-c1">len</span>(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces).strip()) <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span>-<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L490" class="blob-num js-line-number" data-line-number="490"></td>
        <td id="LC490" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> level <span class="pl-k">&gt;=</span> <span class="pl-c1">3</span>:</td>
      </tr>
      <tr>
        <td id="L491" class="blob-num js-line-number" data-line-number="491"></td>
        <td id="LC491" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.insert(<span class="pl-c1">0</span>, level <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span>#<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L492" class="blob-num js-line-number" data-line-number="492"></td>
        <td id="LC492" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># make following text have no gap to the heading:</span></td>
      </tr>
      <tr>
        <td id="L493" class="blob-num js-line-number" data-line-number="493"></td>
        <td id="LC493" class="blob-code blob-code-inner js-file-line">        pieces.extend([<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L494" class="blob-num js-line-number" data-line-number="494"></td>
        <td id="LC494" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> pieces</td>
      </tr>
      <tr>
        <td id="L495" class="blob-num js-line-number" data-line-number="495"></td>
        <td id="LC495" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L496" class="blob-num js-line-number" data-line-number="496"></td>
        <td id="LC496" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_verbatim</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L497" class="blob-num js-line-number" data-line-number="497"></td>
        <td id="LC497" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L498" class="blob-num js-line-number" data-line-number="498"></td>
        <td id="LC498" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L499" class="blob-num js-line-number" data-line-number="499"></td>
        <td id="LC499" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L500" class="blob-num js-line-number" data-line-number="500"></td>
        <td id="LC500" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_blockquote</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L501" class="blob-num js-line-number" data-line-number="501"></td>
        <td id="LC501" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L502" class="blob-num js-line-number" data-line-number="502"></td>
        <td id="LC502" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>&gt; <span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L503" class="blob-num js-line-number" data-line-number="503"></td>
        <td id="LC503" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L504" class="blob-num js-line-number" data-line-number="504"></td>
        <td id="LC504" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_hruler</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L505" class="blob-num js-line-number" data-line-number="505"></td>
        <td id="LC505" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L506" class="blob-num js-line-number" data-line-number="506"></td>
        <td id="LC506" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>* * * * *  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L507" class="blob-num js-line-number" data-line-number="507"></td>
        <td id="LC507" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L508" class="blob-num js-line-number" data-line-number="508"></td>
        <td id="LC508" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_includes</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L509" class="blob-num js-line-number" data-line-number="509"></td>
        <td id="LC509" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>C++ includes: <span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L510" class="blob-num js-line-number" data-line-number="510"></td>
        <td id="LC510" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L511" class="blob-num js-line-number" data-line-number="511"></td>
        <td id="LC511" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L512" class="blob-num js-line-number" data-line-number="512"></td>
        <td id="LC512" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L513" class="blob-num js-line-number" data-line-number="513"></td>
        <td id="LC513" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Para tag handler</span></td>
      </tr>
      <tr>
        <td id="L514" class="blob-num js-line-number" data-line-number="514"></td>
        <td id="LC514" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_para</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L515" class="blob-num js-line-number" data-line-number="515"></td>
        <td id="LC515" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>This is the only place where text wrapping is automatically performed.</span></td>
      </tr>
      <tr>
        <td id="L516" class="blob-num js-line-number" data-line-number="516"></td>
        <td id="LC516" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        Generally, this function parses the node (locally), wraps the text, and</span></td>
      </tr>
      <tr>
        <td id="L517" class="blob-num js-line-number" data-line-number="517"></td>
        <td id="LC517" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        then adds the result to self.pieces. However, it may be convenient to</span></td>
      </tr>
      <tr>
        <td id="L518" class="blob-num js-line-number" data-line-number="518"></td>
        <td id="LC518" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        allow the previous content of self.pieces to be included in the text</span></td>
      </tr>
      <tr>
        <td id="L519" class="blob-num js-line-number" data-line-number="519"></td>
        <td id="LC519" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        wrapping. For this, use the following *convention*:</span></td>
      </tr>
      <tr>
        <td id="L520" class="blob-num js-line-number" data-line-number="520"></td>
        <td id="LC520" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        If self.pieces ends with &#39;&#39;, treat the _previous_ entry as part of the</span></td>
      </tr>
      <tr>
        <td id="L521" class="blob-num js-line-number" data-line-number="521"></td>
        <td id="LC521" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        current paragraph. Else, insert new-line and start a new paragraph</span></td>
      </tr>
      <tr>
        <td id="L522" class="blob-num js-line-number" data-line-number="522"></td>
        <td id="LC522" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        and &quot;wrapping context&quot;.</span></td>
      </tr>
      <tr>
        <td id="L523" class="blob-num js-line-number" data-line-number="523"></td>
        <td id="LC523" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        Paragraphs always end with &#39;  <span class="pl-cce">\n</span>&#39;, but if the parsed content ends with</span></td>
      </tr>
      <tr>
        <td id="L524" class="blob-num js-line-number" data-line-number="524"></td>
        <td id="LC524" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        the special symbol &#39;&#39;, this is passed on.</span></td>
      </tr>
      <tr>
        <td id="L525" class="blob-num js-line-number" data-line-number="525"></td>
        <td id="LC525" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L526" class="blob-num js-line-number" data-line-number="526"></td>
        <td id="LC526" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">==</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L527" class="blob-num js-line-number" data-line-number="527"></td>
        <td id="LC527" class="blob-code blob-code-inner js-file-line">            pieces, <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces[:<span class="pl-k">-</span><span class="pl-c1">2</span>], <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">2</span>:<span class="pl-k">-</span><span class="pl-c1">1</span>]</td>
      </tr>
      <tr>
        <td id="L528" class="blob-num js-line-number" data-line-number="528"></td>
        <td id="LC528" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L529" class="blob-num js-line-number" data-line-number="529"></td>
        <td id="LC529" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L530" class="blob-num js-line-number" data-line-number="530"></td>
        <td id="LC530" class="blob-code blob-code-inner js-file-line">            pieces, <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces, [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L531" class="blob-num js-line-number" data-line-number="531"></td>
        <td id="LC531" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L532" class="blob-num js-line-number" data-line-number="532"></td>
        <td id="LC532" class="blob-code blob-code-inner js-file-line">        dont_end_paragraph <span class="pl-k">=</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">==</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L533" class="blob-num js-line-number" data-line-number="533"></td>
        <td id="LC533" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># Now do the text wrapping:</span></td>
      </tr>
      <tr>
        <td id="L534" class="blob-num js-line-number" data-line-number="534"></td>
        <td id="LC534" class="blob-code blob-code-inner js-file-line">        width <span class="pl-k">=</span> <span class="pl-v">self</span>.textwidth <span class="pl-k">-</span> <span class="pl-v">self</span>.indent</td>
      </tr>
      <tr>
        <td id="L535" class="blob-num js-line-number" data-line-number="535"></td>
        <td id="LC535" class="blob-code blob-code-inner js-file-line">        wrapped_para <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L536" class="blob-num js-line-number" data-line-number="536"></td>
        <td id="LC536" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> line <span class="pl-k">in</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>.join(<span class="pl-v">self</span>.pieces).splitlines():</td>
      </tr>
      <tr>
        <td id="L537" class="blob-num js-line-number" data-line-number="537"></td>
        <td id="LC537" class="blob-code blob-code-inner js-file-line">            keep_markdown_newline <span class="pl-k">=</span> line[<span class="pl-k">-</span><span class="pl-c1">2</span>:] <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L538" class="blob-num js-line-number" data-line-number="538"></td>
        <td id="LC538" class="blob-code blob-code-inner js-file-line">            w_line <span class="pl-k">=</span> textwrap.wrap(line, <span class="pl-smi">width</span><span class="pl-k">=</span>width, <span class="pl-smi">break_long_words</span><span class="pl-k">=</span><span class="pl-c1">False</span>)</td>
      </tr>
      <tr>
        <td id="L539" class="blob-num js-line-number" data-line-number="539"></td>
        <td id="LC539" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> w_line <span class="pl-k">==</span> []:</td>
      </tr>
      <tr>
        <td id="L540" class="blob-num js-line-number" data-line-number="540"></td>
        <td id="LC540" class="blob-code blob-code-inner js-file-line">                w_line <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L541" class="blob-num js-line-number" data-line-number="541"></td>
        <td id="LC541" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> keep_markdown_newline:</td>
      </tr>
      <tr>
        <td id="L542" class="blob-num js-line-number" data-line-number="542"></td>
        <td id="LC542" class="blob-code blob-code-inner js-file-line">                w_line[<span class="pl-k">-</span><span class="pl-c1">1</span>] <span class="pl-k">=</span> w_line[<span class="pl-k">-</span><span class="pl-c1">1</span>] <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L543" class="blob-num js-line-number" data-line-number="543"></td>
        <td id="LC543" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> wl <span class="pl-k">in</span> w_line:</td>
      </tr>
      <tr>
        <td id="L544" class="blob-num js-line-number" data-line-number="544"></td>
        <td id="LC544" class="blob-code blob-code-inner js-file-line">                wrapped_para.append(wl <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L545" class="blob-num js-line-number" data-line-number="545"></td>
        <td id="LC545" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> wrapped_para:</td>
      </tr>
      <tr>
        <td id="L546" class="blob-num js-line-number" data-line-number="546"></td>
        <td id="LC546" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> wrapped_para[<span class="pl-k">-</span><span class="pl-c1">1</span>][<span class="pl-k">-</span><span class="pl-c1">3</span>:] <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L547" class="blob-num js-line-number" data-line-number="547"></td>
        <td id="LC547" class="blob-code blob-code-inner js-file-line">                wrapped_para[<span class="pl-k">-</span><span class="pl-c1">1</span>] <span class="pl-k">=</span> wrapped_para[<span class="pl-k">-</span><span class="pl-c1">1</span>][:<span class="pl-k">-</span><span class="pl-c1">1</span>] <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L548" class="blob-num js-line-number" data-line-number="548"></td>
        <td id="LC548" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> dont_end_paragraph:</td>
      </tr>
      <tr>
        <td id="L549" class="blob-num js-line-number" data-line-number="549"></td>
        <td id="LC549" class="blob-code blob-code-inner js-file-line">                wrapped_para.append(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L550" class="blob-num js-line-number" data-line-number="550"></td>
        <td id="LC550" class="blob-code blob-code-inner js-file-line">        pieces.extend(wrapped_para)</td>
      </tr>
      <tr>
        <td id="L551" class="blob-num js-line-number" data-line-number="551"></td>
        <td id="LC551" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.pieces <span class="pl-k">=</span> pieces</td>
      </tr>
      <tr>
        <td id="L552" class="blob-num js-line-number" data-line-number="552"></td>
        <td id="LC552" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L553" class="blob-num js-line-number" data-line-number="553"></td>
        <td id="LC553" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: List tag handlers</span></td>
      </tr>
      <tr>
        <td id="L554" class="blob-num js-line-number" data-line-number="554"></td>
        <td id="LC554" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_itemizedlist</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L555" class="blob-num js-line-number" data-line-number="555"></td>
        <td id="LC555" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.listitem <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L556" class="blob-num js-line-number" data-line-number="556"></td>
        <td id="LC556" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L557" class="blob-num js-line-number" data-line-number="557"></td>
        <td id="LC557" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> <span class="pl-v">self</span>.pieces <span class="pl-k">!=</span> [] <span class="pl-k">and</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">!=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L558" class="blob-num js-line-number" data-line-number="558"></td>
        <td id="LC558" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L559" class="blob-num js-line-number" data-line-number="559"></td>
        <td id="LC559" class="blob-code blob-code-inner js-file-line">        listitem <span class="pl-k">=</span> <span class="pl-v">self</span>.listitem</td>
      </tr>
      <tr>
        <td id="L560" class="blob-num js-line-number" data-line-number="560"></td>
        <td id="LC560" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.listitem <span class="pl-k">in</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>*<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>-<span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L561" class="blob-num js-line-number" data-line-number="561"></td>
        <td id="LC561" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>-<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L562" class="blob-num js-line-number" data-line-number="562"></td>
        <td id="LC562" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L563" class="blob-num js-line-number" data-line-number="563"></td>
        <td id="LC563" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>*<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L564" class="blob-num js-line-number" data-line-number="564"></td>
        <td id="LC564" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L565" class="blob-num js-line-number" data-line-number="565"></td>
        <td id="LC565" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> listitem</td>
      </tr>
      <tr>
        <td id="L566" class="blob-num js-line-number" data-line-number="566"></td>
        <td id="LC566" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L567" class="blob-num js-line-number" data-line-number="567"></td>
        <td id="LC567" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_orderedlist</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L568" class="blob-num js-line-number" data-line-number="568"></td>
        <td id="LC568" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.listitem <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L569" class="blob-num js-line-number" data-line-number="569"></td>
        <td id="LC569" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L570" class="blob-num js-line-number" data-line-number="570"></td>
        <td id="LC570" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> <span class="pl-v">self</span>.pieces <span class="pl-k">!=</span> [] <span class="pl-k">and</span> <span class="pl-v">self</span>.pieces[<span class="pl-k">-</span><span class="pl-c1">1</span>:] <span class="pl-k">!=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L571" class="blob-num js-line-number" data-line-number="571"></td>
        <td id="LC571" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L572" class="blob-num js-line-number" data-line-number="572"></td>
        <td id="LC572" class="blob-code blob-code-inner js-file-line">        listitem <span class="pl-k">=</span> <span class="pl-v">self</span>.listitem</td>
      </tr>
      <tr>
        <td id="L573" class="blob-num js-line-number" data-line-number="573"></td>
        <td id="LC573" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> <span class="pl-c1">0</span></td>
      </tr>
      <tr>
        <td id="L574" class="blob-num js-line-number" data-line-number="574"></td>
        <td id="LC574" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L575" class="blob-num js-line-number" data-line-number="575"></td>
        <td id="LC575" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> listitem</td>
      </tr>
      <tr>
        <td id="L576" class="blob-num js-line-number" data-line-number="576"></td>
        <td id="LC576" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L577" class="blob-num js-line-number" data-line-number="577"></td>
        <td id="LC577" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_listitem</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L578" class="blob-num js-line-number" data-line-number="578"></td>
        <td id="LC578" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">try</span>:</td>
      </tr>
      <tr>
        <td id="L579" class="blob-num js-line-number" data-line-number="579"></td>
        <td id="LC579" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.listitem <span class="pl-k">=</span> <span class="pl-c1">int</span>(<span class="pl-v">self</span>.listitem) <span class="pl-k">+</span> <span class="pl-c1">1</span></td>
      </tr>
      <tr>
        <td id="L580" class="blob-num js-line-number" data-line-number="580"></td>
        <td id="LC580" class="blob-code blob-code-inner js-file-line">            item <span class="pl-k">=</span> <span class="pl-c1">str</span>(<span class="pl-v">self</span>.listitem) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>. <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L581" class="blob-num js-line-number" data-line-number="581"></td>
        <td id="LC581" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">except</span>:</td>
      </tr>
      <tr>
        <td id="L582" class="blob-num js-line-number" data-line-number="582"></td>
        <td id="LC582" class="blob-code blob-code-inner js-file-line">            item <span class="pl-k">=</span> <span class="pl-c1">str</span>(<span class="pl-v">self</span>.listitem) <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span> <span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L583" class="blob-num js-line-number" data-line-number="583"></td>
        <td id="LC583" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node, item, <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L584" class="blob-num js-line-number" data-line-number="584"></td>
        <td id="LC584" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L585" class="blob-num js-line-number" data-line-number="585"></td>
        <td id="LC585" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Parameter list tag handlers</span></td>
      </tr>
      <tr>
        <td id="L586" class="blob-num js-line-number" data-line-number="586"></td>
        <td id="LC586" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_parameterlist</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L587" class="blob-num js-line-number" data-line-number="587"></td>
        <td id="LC587" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L588" class="blob-num js-line-number" data-line-number="588"></td>
        <td id="LC588" class="blob-code blob-code-inner js-file-line">        text <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>unknown<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L589" class="blob-num js-line-number" data-line-number="589"></td>
        <td id="LC589" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> key, val <span class="pl-k">in</span> node.attributes.items():</td>
      </tr>
      <tr>
        <td id="L590" class="blob-num js-line-number" data-line-number="590"></td>
        <td id="LC590" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> key <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L591" class="blob-num js-line-number" data-line-number="591"></td>
        <td id="LC591" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">if</span> val <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>param<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L592" class="blob-num js-line-number" data-line-number="592"></td>
        <td id="LC592" class="blob-code blob-code-inner js-file-line">                    text <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>Parameters<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L593" class="blob-num js-line-number" data-line-number="593"></td>
        <td id="LC593" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">elif</span> val <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>exception<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L594" class="blob-num js-line-number" data-line-number="594"></td>
        <td id="LC594" class="blob-code blob-code-inner js-file-line">                    text <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>Exceptions<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L595" class="blob-num js-line-number" data-line-number="595"></td>
        <td id="LC595" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">elif</span> val <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>retval<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L596" class="blob-num js-line-number" data-line-number="596"></td>
        <td id="LC596" class="blob-code blob-code-inner js-file-line">                    text <span class="pl-k">=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>Returns<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L597" class="blob-num js-line-number" data-line-number="597"></td>
        <td id="LC597" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L598" class="blob-num js-line-number" data-line-number="598"></td>
        <td id="LC598" class="blob-code blob-code-inner js-file-line">                    text <span class="pl-k">=</span> val</td>
      </tr>
      <tr>
        <td id="L599" class="blob-num js-line-number" data-line-number="599"></td>
        <td id="LC599" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">break</span></td>
      </tr>
      <tr>
        <td id="L600" class="blob-num js-line-number" data-line-number="600"></td>
        <td id="LC600" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.indent <span class="pl-k">==</span> <span class="pl-c1">0</span>:</td>
      </tr>
      <tr>
        <td id="L601" class="blob-num js-line-number" data-line-number="601"></td>
        <td id="LC601" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([text, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-c1">len</span>(text) <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span>-<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L602" class="blob-num js-line-number" data-line-number="602"></td>
        <td id="LC602" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L603" class="blob-num js-line-number" data-line-number="603"></td>
        <td id="LC603" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([text, <span class="pl-s"><span class="pl-pds">&#39;</span>:  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L604" class="blob-num js-line-number" data-line-number="604"></td>
        <td id="LC604" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L605" class="blob-num js-line-number" data-line-number="605"></td>
        <td id="LC605" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L606" class="blob-num js-line-number" data-line-number="606"></td>
        <td id="LC606" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_parameteritem</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L607" class="blob-num js-line-number" data-line-number="607"></td>
        <td id="LC607" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>* <span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L608" class="blob-num js-line-number" data-line-number="608"></td>
        <td id="LC608" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L609" class="blob-num js-line-number" data-line-number="609"></td>
        <td id="LC609" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_parameternamelist</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L610" class="blob-num js-line-number" data-line-number="610"></td>
        <td id="LC610" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L611" class="blob-num js-line-number" data-line-number="611"></td>
        <td id="LC611" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span> :<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>  <span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L612" class="blob-num js-line-number" data-line-number="612"></td>
        <td id="LC612" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L613" class="blob-num js-line-number" data-line-number="613"></td>
        <td id="LC613" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_parametername</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L614" class="blob-num js-line-number" data-line-number="614"></td>
        <td id="LC614" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-v">self</span>.pieces <span class="pl-k">!=</span> [] <span class="pl-k">and</span> <span class="pl-v">self</span>.pieces <span class="pl-k">!=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>* <span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L615" class="blob-num js-line-number" data-line-number="615"></td>
        <td id="LC615" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span>, <span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L616" class="blob-num js-line-number" data-line-number="616"></td>
        <td id="LC616" class="blob-code blob-code-inner js-file-line">        data <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(node)</td>
      </tr>
      <tr>
        <td id="L617" class="blob-num js-line-number" data-line-number="617"></td>
        <td id="LC617" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>, data, <span class="pl-s"><span class="pl-pds">&#39;</span>`<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L618" class="blob-num js-line-number" data-line-number="618"></td>
        <td id="LC618" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L619" class="blob-num js-line-number" data-line-number="619"></td>
        <td id="LC619" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_parameterdescription</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L620" class="blob-num js-line-number" data-line-number="620"></td>
        <td id="LC620" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L621" class="blob-num js-line-number" data-line-number="621"></td>
        <td id="LC621" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L622" class="blob-num js-line-number" data-line-number="622"></td>
        <td id="LC622" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Section tag handler</span></td>
      </tr>
      <tr>
        <td id="L623" class="blob-num js-line-number" data-line-number="623"></td>
        <td id="LC623" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_simplesect</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L624" class="blob-num js-line-number" data-line-number="624"></td>
        <td id="LC624" class="blob-code blob-code-inner js-file-line">        kind <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L625" class="blob-num js-line-number" data-line-number="625"></td>
        <td id="LC625" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>date<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>rcs<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>version<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L626" class="blob-num js-line-number" data-line-number="626"></td>
        <td id="LC626" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L627" class="blob-num js-line-number" data-line-number="627"></td>
        <td id="LC627" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.start_new_paragraph()</td>
      </tr>
      <tr>
        <td id="L628" class="blob-num js-line-number" data-line-number="628"></td>
        <td id="LC628" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>warning<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L629" class="blob-num js-line-number" data-line-number="629"></td>
        <td id="LC629" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>**Warning**: <span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L630" class="blob-num js-line-number" data-line-number="630"></td>
        <td id="LC630" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> kind <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>see<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L631" class="blob-num js-line-number" data-line-number="631"></td>
        <td id="LC631" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[<span class="pl-s"><span class="pl-pds">&#39;</span>See also: <span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L632" class="blob-num js-line-number" data-line-number="632"></td>
        <td id="LC632" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> kind <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>return<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L633" class="blob-num js-line-number" data-line-number="633"></td>
        <td id="LC633" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.indent <span class="pl-k">==</span> <span class="pl-c1">0</span>:</td>
      </tr>
      <tr>
        <td id="L634" class="blob-num js-line-number" data-line-number="634"></td>
        <td id="LC634" class="blob-code blob-code-inner js-file-line">                pieces <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>Returns<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-c1">len</span>(<span class="pl-s"><span class="pl-pds">&#39;</span>Returns<span class="pl-pds">&#39;</span></span>) <span class="pl-k">*</span> <span class="pl-s"><span class="pl-pds">&#39;</span>-<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L635" class="blob-num js-line-number" data-line-number="635"></td>
        <td id="LC635" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L636" class="blob-num js-line-number" data-line-number="636"></td>
        <td id="LC636" class="blob-code blob-code-inner js-file-line">                pieces <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>Returns:<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L637" class="blob-num js-line-number" data-line-number="637"></td>
        <td id="LC637" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>pieces)</td>
      </tr>
      <tr>
        <td id="L638" class="blob-num js-line-number" data-line-number="638"></td>
        <td id="LC638" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">else</span>:</td>
      </tr>
      <tr>
        <td id="L639" class="blob-num js-line-number" data-line-number="639"></td>
        <td id="LC639" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">pieces</span><span class="pl-k">=</span>[kind <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>: <span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>], <span class="pl-smi">indent</span><span class="pl-k">=</span><span class="pl-c1">4</span>)</td>
      </tr>
      <tr>
        <td id="L640" class="blob-num js-line-number" data-line-number="640"></td>
        <td id="LC640" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L641" class="blob-num js-line-number" data-line-number="641"></td>
        <td id="LC641" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: %feature(&quot;docstring&quot;) producing tag handlers</span></td>
      </tr>
      <tr>
        <td id="L642" class="blob-num js-line-number" data-line-number="642"></td>
        <td id="LC642" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_compounddef</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L643" class="blob-num js-line-number" data-line-number="643"></td>
        <td id="LC643" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>This produces <span class="pl-c1">%f</span>eature(&quot;docstring&quot;) entries for classes, and handles</span></td>
      </tr>
      <tr>
        <td id="L644" class="blob-num js-line-number" data-line-number="644"></td>
        <td id="LC644" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        class, namespace and file memberdef entries specially to allow for </span></td>
      </tr>
      <tr>
        <td id="L645" class="blob-num js-line-number" data-line-number="645"></td>
        <td id="LC645" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        overloaded functions. For other cases, passes parsing on to standard</span></td>
      </tr>
      <tr>
        <td id="L646" class="blob-num js-line-number" data-line-number="646"></td>
        <td id="LC646" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        handlers (which may produce unexpected results).</span></td>
      </tr>
      <tr>
        <td id="L647" class="blob-num js-line-number" data-line-number="647"></td>
        <td id="LC647" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L648" class="blob-num js-line-number" data-line-number="648"></td>
        <td id="LC648" class="blob-code blob-code-inner js-file-line">        kind <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L649" class="blob-num js-line-number" data-line-number="649"></td>
        <td id="LC649" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>class<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>struct<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L650" class="blob-num js-line-number" data-line-number="650"></td>
        <td id="LC650" class="blob-code blob-code-inner js-file-line">            prot <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>prot<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L651" class="blob-num js-line-number" data-line-number="651"></td>
        <td id="LC651" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> prot <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>public<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L652" class="blob-num js-line-number" data-line-number="652"></td>
        <td id="LC652" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L653" class="blob-num js-line-number" data-line-number="653"></td>
        <td id="LC653" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L654" class="blob-num js-line-number" data-line-number="654"></td>
        <td id="LC654" class="blob-code blob-code-inner js-file-line">            classdefn <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>compoundname<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L655" class="blob-num js-line-number" data-line-number="655"></td>
        <td id="LC655" class="blob-code blob-code-inner js-file-line">            classname <span class="pl-k">=</span> classdefn.split(<span class="pl-s"><span class="pl-pds">&#39;</span>::<span class="pl-pds">&#39;</span></span>)[<span class="pl-k">-</span><span class="pl-c1">1</span>]</td>
      </tr>
      <tr>
        <td id="L656" class="blob-num js-line-number" data-line-number="656"></td>
        <td id="LC656" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-c1">%%</span>feature(&quot;docstring&quot;) <span class="pl-c1">%s</span> &quot;<span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span> <span class="pl-k">%</span> classdefn)</td>
      </tr>
      <tr>
        <td id="L657" class="blob-num js-line-number" data-line-number="657"></td>
        <td id="LC657" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L658" class="blob-num js-line-number" data-line-number="658"></td>
        <td id="LC658" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.with_constructor_list:</td>
      </tr>
      <tr>
        <td id="L659" class="blob-num js-line-number" data-line-number="659"></td>
        <td id="LC659" class="blob-code blob-code-inner js-file-line">                constructor_nodes <span class="pl-k">=</span> []</td>
      </tr>
      <tr>
        <td id="L660" class="blob-num js-line-number" data-line-number="660"></td>
        <td id="LC660" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">for</span> n <span class="pl-k">in</span> <span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>memberdef<span class="pl-pds">&#39;</span></span>, <span class="pl-smi">recursive</span><span class="pl-k">=</span><span class="pl-c1">2</span>):</td>
      </tr>
      <tr>
        <td id="L661" class="blob-num js-line-number" data-line-number="661"></td>
        <td id="LC661" class="blob-code blob-code-inner js-file-line">                    <span class="pl-k">if</span> n.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>prot<span class="pl-pds">&#39;</span></span>].value <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>public<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L662" class="blob-num js-line-number" data-line-number="662"></td>
        <td id="LC662" class="blob-code blob-code-inner js-file-line">                        <span class="pl-k">if</span> <span class="pl-v">self</span>.extract_text(<span class="pl-v">self</span>.get_specific_subnodes(n, <span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>)) <span class="pl-k">==</span> classdefn <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>::<span class="pl-pds">&#39;</span></span> <span class="pl-k">+</span> classname:</td>
      </tr>
      <tr>
        <td id="L663" class="blob-num js-line-number" data-line-number="663"></td>
        <td id="LC663" class="blob-code blob-code-inner js-file-line">                            constructor_nodes.append(n)</td>
      </tr>
      <tr>
        <td id="L664" class="blob-num js-line-number" data-line-number="664"></td>
        <td id="LC664" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">for</span> n <span class="pl-k">in</span> constructor_nodes:</td>
      </tr>
      <tr>
        <td id="L665" class="blob-num js-line-number" data-line-number="665"></td>
        <td id="LC665" class="blob-code blob-code-inner js-file-line">                    <span class="pl-v">self</span>.add_line_with_subsequent_indent(<span class="pl-v">self</span>.get_function_signature(n))</td>
      </tr>
      <tr>
        <td id="L666" class="blob-num js-line-number" data-line-number="666"></td>
        <td id="LC666" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L667" class="blob-num js-line-number" data-line-number="667"></td>
        <td id="LC667" class="blob-code blob-code-inner js-file-line">            names <span class="pl-k">=</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>briefdescription<span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span>detaileddescription<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L668" class="blob-num js-line-number" data-line-number="668"></td>
        <td id="LC668" class="blob-code blob-code-inner js-file-line">            sub_dict <span class="pl-k">=</span> <span class="pl-v">self</span>.get_specific_nodes(node, names)</td>
      </tr>
      <tr>
        <td id="L669" class="blob-num js-line-number" data-line-number="669"></td>
        <td id="LC669" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> n <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>briefdescription<span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span>detaileddescription<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L670" class="blob-num js-line-number" data-line-number="670"></td>
        <td id="LC670" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">if</span> n <span class="pl-k">in</span> sub_dict:</td>
      </tr>
      <tr>
        <td id="L671" class="blob-num js-line-number" data-line-number="671"></td>
        <td id="LC671" class="blob-code blob-code-inner js-file-line">                    <span class="pl-v">self</span>.parse(sub_dict[n])</td>
      </tr>
      <tr>
        <td id="L672" class="blob-num js-line-number" data-line-number="672"></td>
        <td id="LC672" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.with_constructor_list:</td>
      </tr>
      <tr>
        <td id="L673" class="blob-num js-line-number" data-line-number="673"></td>
        <td id="LC673" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.make_constructor_list(constructor_nodes, classname)</td>
      </tr>
      <tr>
        <td id="L674" class="blob-num js-line-number" data-line-number="674"></td>
        <td id="LC674" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-v">self</span>.with_attribute_list:</td>
      </tr>
      <tr>
        <td id="L675" class="blob-num js-line-number" data-line-number="675"></td>
        <td id="LC675" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.make_attribute_list(node)</td>
      </tr>
      <tr>
        <td id="L676" class="blob-num js-line-number" data-line-number="676"></td>
        <td id="LC676" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L677" class="blob-num js-line-number" data-line-number="677"></td>
        <td id="LC677" class="blob-code blob-code-inner js-file-line">            sub_list <span class="pl-k">=</span> <span class="pl-v">self</span>.get_specific_subnodes(node, <span class="pl-s"><span class="pl-pds">&#39;</span>includes<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L678" class="blob-num js-line-number" data-line-number="678"></td>
        <td id="LC678" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> sub_list:</td>
      </tr>
      <tr>
        <td id="L679" class="blob-num js-line-number" data-line-number="679"></td>
        <td id="LC679" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.parse(sub_list[<span class="pl-c1">0</span>])</td>
      </tr>
      <tr>
        <td id="L680" class="blob-num js-line-number" data-line-number="680"></td>
        <td id="LC680" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>&quot;;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L681" class="blob-num js-line-number" data-line-number="681"></td>
        <td id="LC681" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L682" class="blob-num js-line-number" data-line-number="682"></td>
        <td id="LC682" class="blob-code blob-code-inner js-file-line">            names <span class="pl-k">=</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>compoundname<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>briefdescription<span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span>detaileddescription<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>includes<span class="pl-pds">&#39;</span></span>]</td>
      </tr>
      <tr>
        <td id="L683" class="blob-num js-line-number" data-line-number="683"></td>
        <td id="LC683" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node, <span class="pl-smi">ignore</span> <span class="pl-k">=</span> names)</td>
      </tr>
      <tr>
        <td id="L684" class="blob-num js-line-number" data-line-number="684"></td>
        <td id="LC684" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L685" class="blob-num js-line-number" data-line-number="685"></td>
        <td id="LC685" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">elif</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>file<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L686" class="blob-num js-line-number" data-line-number="686"></td>
        <td id="LC686" class="blob-code blob-code-inner js-file-line">            nodes <span class="pl-k">=</span> node.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>sectiondef<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L687" class="blob-num js-line-number" data-line-number="687"></td>
        <td id="LC687" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> n <span class="pl-k">in</span> nodes:</td>
      </tr>
      <tr>
        <td id="L688" class="blob-num js-line-number" data-line-number="688"></td>
        <td id="LC688" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.parse(n)</td>
      </tr>
      <tr>
        <td id="L689" class="blob-num js-line-number" data-line-number="689"></td>
        <td id="LC689" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L690" class="blob-num js-line-number" data-line-number="690"></td>
        <td id="LC690" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># now explicitely handle possibly overloaded member functions.</span></td>
      </tr>
      <tr>
        <td id="L691" class="blob-num js-line-number" data-line-number="691"></td>
        <td id="LC691" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">in</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>class<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>struct<span class="pl-pds">&#39;</span></span>,<span class="pl-s"><span class="pl-pds">&#39;</span>file<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L692" class="blob-num js-line-number" data-line-number="692"></td>
        <td id="LC692" class="blob-code blob-code-inner js-file-line">            md_nodes <span class="pl-k">=</span> <span class="pl-v">self</span>.get_memberdef_nodes_and_signatures(node, kind)</td>
      </tr>
      <tr>
        <td id="L693" class="blob-num js-line-number" data-line-number="693"></td>
        <td id="LC693" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">for</span> sig <span class="pl-k">in</span> md_nodes:</td>
      </tr>
      <tr>
        <td id="L694" class="blob-num js-line-number" data-line-number="694"></td>
        <td id="LC694" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.handle_typical_memberdefs(sig, md_nodes[sig])</td>
      </tr>
      <tr>
        <td id="L695" class="blob-num js-line-number" data-line-number="695"></td>
        <td id="LC695" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L696" class="blob-num js-line-number" data-line-number="696"></td>
        <td id="LC696" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_memberdef</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L697" class="blob-num js-line-number" data-line-number="697"></td>
        <td id="LC697" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>Handle cases outside of class, struct, file or namespace. These are</span></td>
      </tr>
      <tr>
        <td id="L698" class="blob-num js-line-number" data-line-number="698"></td>
        <td id="LC698" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        now dealt with by `handle_overloaded_memberfunction`.</span></td>
      </tr>
      <tr>
        <td id="L699" class="blob-num js-line-number" data-line-number="699"></td>
        <td id="LC699" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        Do these even exist???</span></td>
      </tr>
      <tr>
        <td id="L700" class="blob-num js-line-number" data-line-number="700"></td>
        <td id="LC700" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        <span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L701" class="blob-num js-line-number" data-line-number="701"></td>
        <td id="LC701" class="blob-code blob-code-inner js-file-line">        prot <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>prot<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L702" class="blob-num js-line-number" data-line-number="702"></td>
        <td id="LC702" class="blob-code blob-code-inner js-file-line">        <span class="pl-c1">id</span> <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>id<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L703" class="blob-num js-line-number" data-line-number="703"></td>
        <td id="LC703" class="blob-code blob-code-inner js-file-line">        kind <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L704" class="blob-num js-line-number" data-line-number="704"></td>
        <td id="LC704" class="blob-code blob-code-inner js-file-line">        tmp <span class="pl-k">=</span> node.parentNode.parentNode.parentNode</td>
      </tr>
      <tr>
        <td id="L705" class="blob-num js-line-number" data-line-number="705"></td>
        <td id="LC705" class="blob-code blob-code-inner js-file-line">        compdef <span class="pl-k">=</span> tmp.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>compounddef<span class="pl-pds">&#39;</span></span>)[<span class="pl-c1">0</span>]</td>
      </tr>
      <tr>
        <td id="L706" class="blob-num js-line-number" data-line-number="706"></td>
        <td id="LC706" class="blob-code blob-code-inner js-file-line">        cdef_kind <span class="pl-k">=</span> compdef.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L707" class="blob-num js-line-number" data-line-number="707"></td>
        <td id="LC707" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> cdef_kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>file<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>class<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>struct<span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L708" class="blob-num js-line-number" data-line-number="708"></td>
        <td id="LC708" class="blob-code blob-code-inner js-file-line">            <span class="pl-c"># These cases are now handled by `handle_typical_memberdefs`</span></td>
      </tr>
      <tr>
        <td id="L709" class="blob-num js-line-number" data-line-number="709"></td>
        <td id="LC709" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L710" class="blob-num js-line-number" data-line-number="710"></td>
        <td id="LC710" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> prot <span class="pl-k">!=</span> <span class="pl-s"><span class="pl-pds">&#39;</span>public<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L711" class="blob-num js-line-number" data-line-number="711"></td>
        <td id="LC711" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L712" class="blob-num js-line-number" data-line-number="712"></td>
        <td id="LC712" class="blob-code blob-code-inner js-file-line">        first <span class="pl-k">=</span> <span class="pl-v">self</span>.get_specific_nodes(node, (<span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>))</td>
      </tr>
      <tr>
        <td id="L713" class="blob-num js-line-number" data-line-number="713"></td>
        <td id="LC713" class="blob-code blob-code-inner js-file-line">        name <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(first[<span class="pl-s"><span class="pl-pds">&#39;</span>name<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L714" class="blob-num js-line-number" data-line-number="714"></td>
        <td id="LC714" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> name[:<span class="pl-c1">8</span>] <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>operator<span class="pl-pds">&#39;</span></span>:  <span class="pl-c"># Don&#39;t handle operators yet.</span></td>
      </tr>
      <tr>
        <td id="L715" class="blob-num js-line-number" data-line-number="715"></td>
        <td id="LC715" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L716" class="blob-num js-line-number" data-line-number="716"></td>
        <td id="LC716" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-k">not</span> <span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span> <span class="pl-k">in</span> first <span class="pl-k">or</span> kind <span class="pl-k">in</span> [<span class="pl-s"><span class="pl-pds">&#39;</span>variable<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>typedef<span class="pl-pds">&#39;</span></span>]:</td>
      </tr>
      <tr>
        <td id="L717" class="blob-num js-line-number" data-line-number="717"></td>
        <td id="LC717" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">return</span></td>
      </tr>
      <tr>
        <td id="L718" class="blob-num js-line-number" data-line-number="718"></td>
        <td id="LC718" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L719" class="blob-num js-line-number" data-line-number="719"></td>
        <td id="LC719" class="blob-code blob-code-inner js-file-line">        data <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(first[<span class="pl-s"><span class="pl-pds">&#39;</span>definition<span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L720" class="blob-num js-line-number" data-line-number="720"></td>
        <td id="LC720" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L721" class="blob-num js-line-number" data-line-number="721"></td>
        <td id="LC721" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>/* where did this entry come from??? */<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L722" class="blob-num js-line-number" data-line-number="722"></td>
        <td id="LC722" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-c1">%f</span>eature(&quot;docstring&quot;) <span class="pl-c1">%s</span> &quot;<span class="pl-cce">\n</span><span class="pl-c1">%s</span><span class="pl-pds">&#39;</span></span> <span class="pl-k">%</span> (data, data))</td>
      </tr>
      <tr>
        <td id="L723" class="blob-num js-line-number" data-line-number="723"></td>
        <td id="LC723" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L724" class="blob-num js-line-number" data-line-number="724"></td>
        <td id="LC724" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> n <span class="pl-k">in</span> node.childNodes:</td>
      </tr>
      <tr>
        <td id="L725" class="blob-num js-line-number" data-line-number="725"></td>
        <td id="LC725" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> n <span class="pl-k">not</span> <span class="pl-k">in</span> first.values():</td>
      </tr>
      <tr>
        <td id="L726" class="blob-num js-line-number" data-line-number="726"></td>
        <td id="LC726" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.parse(n)</td>
      </tr>
      <tr>
        <td id="L727" class="blob-num js-line-number" data-line-number="727"></td>
        <td id="LC727" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text([<span class="pl-s"><span class="pl-pds">&#39;</span>&quot;;<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>])</td>
      </tr>
      <tr>
        <td id="L728" class="blob-num js-line-number" data-line-number="728"></td>
        <td id="LC728" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L729" class="blob-num js-line-number" data-line-number="729"></td>
        <td id="LC729" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: Entry tag handlers (dont print anything meaningful)</span></td>
      </tr>
      <tr>
        <td id="L730" class="blob-num js-line-number" data-line-number="730"></td>
        <td id="LC730" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_sectiondef</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L731" class="blob-num js-line-number" data-line-number="731"></td>
        <td id="LC731" class="blob-code blob-code-inner js-file-line">        kind <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L732" class="blob-num js-line-number" data-line-number="732"></td>
        <td id="LC732" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">in</span> (<span class="pl-s"><span class="pl-pds">&#39;</span>public-func<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>func<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>user-defined<span class="pl-pds">&#39;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-pds">&#39;</span></span>):</td>
      </tr>
      <tr>
        <td id="L733" class="blob-num js-line-number" data-line-number="733"></td>
        <td id="LC733" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L734" class="blob-num js-line-number" data-line-number="734"></td>
        <td id="LC734" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L735" class="blob-num js-line-number" data-line-number="735"></td>
        <td id="LC735" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_header</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L736" class="blob-num js-line-number" data-line-number="736"></td>
        <td id="LC736" class="blob-code blob-code-inner js-file-line">        <span class="pl-s"><span class="pl-pds">&quot;&quot;&quot;</span>For a user defined section def a header field is present</span></td>
      </tr>
      <tr>
        <td id="L737" class="blob-num js-line-number" data-line-number="737"></td>
        <td id="LC737" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        which should not be printed as such, so we comment it in the</span></td>
      </tr>
      <tr>
        <td id="L738" class="blob-num js-line-number" data-line-number="738"></td>
        <td id="LC738" class="blob-code blob-code-inner js-file-line"><span class="pl-s">        output.<span class="pl-pds">&quot;&quot;&quot;</span></span></td>
      </tr>
      <tr>
        <td id="L739" class="blob-num js-line-number" data-line-number="739"></td>
        <td id="LC739" class="blob-code blob-code-inner js-file-line">        data <span class="pl-k">=</span> <span class="pl-v">self</span>.extract_text(node)</td>
      </tr>
      <tr>
        <td id="L740" class="blob-num js-line-number" data-line-number="740"></td>
        <td id="LC740" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>/*<span class="pl-cce">\n</span> <span class="pl-c1">%s</span> <span class="pl-cce">\n</span>*/<span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span> <span class="pl-k">%</span> data)</td>
      </tr>
      <tr>
        <td id="L741" class="blob-num js-line-number" data-line-number="741"></td>
        <td id="LC741" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># If our immediate sibling is a &#39;description&#39; node then we</span></td>
      </tr>
      <tr>
        <td id="L742" class="blob-num js-line-number" data-line-number="742"></td>
        <td id="LC742" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># should comment that out also and remove it from the parent</span></td>
      </tr>
      <tr>
        <td id="L743" class="blob-num js-line-number" data-line-number="743"></td>
        <td id="LC743" class="blob-code blob-code-inner js-file-line">        <span class="pl-c"># node&#39;s children.</span></td>
      </tr>
      <tr>
        <td id="L744" class="blob-num js-line-number" data-line-number="744"></td>
        <td id="LC744" class="blob-code blob-code-inner js-file-line">        parent <span class="pl-k">=</span> node.parentNode</td>
      </tr>
      <tr>
        <td id="L745" class="blob-num js-line-number" data-line-number="745"></td>
        <td id="LC745" class="blob-code blob-code-inner js-file-line">        idx <span class="pl-k">=</span> parent.childNodes.index(node)</td>
      </tr>
      <tr>
        <td id="L746" class="blob-num js-line-number" data-line-number="746"></td>
        <td id="LC746" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> <span class="pl-c1">len</span>(parent.childNodes) <span class="pl-k">&gt;=</span> idx <span class="pl-k">+</span> <span class="pl-c1">2</span>:</td>
      </tr>
      <tr>
        <td id="L747" class="blob-num js-line-number" data-line-number="747"></td>
        <td id="LC747" class="blob-code blob-code-inner js-file-line">            nd <span class="pl-k">=</span> parent.childNodes[idx <span class="pl-k">+</span> <span class="pl-c1">2</span>]</td>
      </tr>
      <tr>
        <td id="L748" class="blob-num js-line-number" data-line-number="748"></td>
        <td id="LC748" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> nd.nodeName <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>description<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L749" class="blob-num js-line-number" data-line-number="749"></td>
        <td id="LC749" class="blob-code blob-code-inner js-file-line">                nd <span class="pl-k">=</span> parent.removeChild(nd)</td>
      </tr>
      <tr>
        <td id="L750" class="blob-num js-line-number" data-line-number="750"></td>
        <td id="LC750" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>/*<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L751" class="blob-num js-line-number" data-line-number="751"></td>
        <td id="LC751" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.subnode_parse(nd)</td>
      </tr>
      <tr>
        <td id="L752" class="blob-num js-line-number" data-line-number="752"></td>
        <td id="LC752" class="blob-code blob-code-inner js-file-line">                <span class="pl-v">self</span>.add_text(<span class="pl-s"><span class="pl-pds">&#39;</span><span class="pl-cce">\n</span>*/<span class="pl-cce">\n</span><span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L753" class="blob-num js-line-number" data-line-number="753"></td>
        <td id="LC753" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L754" class="blob-num js-line-number" data-line-number="754"></td>
        <td id="LC754" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_member</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L755" class="blob-num js-line-number" data-line-number="755"></td>
        <td id="LC755" class="blob-code blob-code-inner js-file-line">        kind <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>kind<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L756" class="blob-num js-line-number" data-line-number="756"></td>
        <td id="LC756" class="blob-code blob-code-inner js-file-line">        refid <span class="pl-k">=</span> node.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>refid<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L757" class="blob-num js-line-number" data-line-number="757"></td>
        <td id="LC757" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">if</span> kind <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>function<span class="pl-pds">&#39;</span></span> <span class="pl-k">and</span> refid[:<span class="pl-c1">9</span>] <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>namespace<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L758" class="blob-num js-line-number" data-line-number="758"></td>
        <td id="LC758" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.subnode_parse(node)</td>
      </tr>
      <tr>
        <td id="L759" class="blob-num js-line-number" data-line-number="759"></td>
        <td id="LC759" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L760" class="blob-num js-line-number" data-line-number="760"></td>
        <td id="LC760" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">def</span> <span class="pl-en">do_doxygenindex</span>(<span class="pl-smi">self</span>, <span class="pl-smi">node</span>):</td>
      </tr>
      <tr>
        <td id="L761" class="blob-num js-line-number" data-line-number="761"></td>
        <td id="LC761" class="blob-code blob-code-inner js-file-line">        <span class="pl-v">self</span>.multi <span class="pl-k">=</span> <span class="pl-c1">1</span></td>
      </tr>
      <tr>
        <td id="L762" class="blob-num js-line-number" data-line-number="762"></td>
        <td id="LC762" class="blob-code blob-code-inner js-file-line">        comps <span class="pl-k">=</span> node.getElementsByTagName(<span class="pl-s"><span class="pl-pds">&#39;</span>compound<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L763" class="blob-num js-line-number" data-line-number="763"></td>
        <td id="LC763" class="blob-code blob-code-inner js-file-line">        <span class="pl-k">for</span> c <span class="pl-k">in</span> comps:</td>
      </tr>
      <tr>
        <td id="L764" class="blob-num js-line-number" data-line-number="764"></td>
        <td id="LC764" class="blob-code blob-code-inner js-file-line">            refid <span class="pl-k">=</span> c.attributes[<span class="pl-s"><span class="pl-pds">&#39;</span>refid<span class="pl-pds">&#39;</span></span>].value</td>
      </tr>
      <tr>
        <td id="L765" class="blob-num js-line-number" data-line-number="765"></td>
        <td id="LC765" class="blob-code blob-code-inner js-file-line">            fname <span class="pl-k">=</span> refid <span class="pl-k">+</span> <span class="pl-s"><span class="pl-pds">&#39;</span>.xml<span class="pl-pds">&#39;</span></span></td>
      </tr>
      <tr>
        <td id="L766" class="blob-num js-line-number" data-line-number="766"></td>
        <td id="LC766" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-k">not</span> os.path.exists(fname):</td>
      </tr>
      <tr>
        <td id="L767" class="blob-num js-line-number" data-line-number="767"></td>
        <td id="LC767" class="blob-code blob-code-inner js-file-line">                fname <span class="pl-k">=</span> os.path.join(<span class="pl-v">self</span>.my_dir,  fname)</td>
      </tr>
      <tr>
        <td id="L768" class="blob-num js-line-number" data-line-number="768"></td>
        <td id="LC768" class="blob-code blob-code-inner js-file-line">            <span class="pl-k">if</span> <span class="pl-k">not</span> <span class="pl-v">self</span>.quiet:</td>
      </tr>
      <tr>
        <td id="L769" class="blob-num js-line-number" data-line-number="769"></td>
        <td id="LC769" class="blob-code blob-code-inner js-file-line">                <span class="pl-k">print</span>(<span class="pl-s"><span class="pl-pds">&quot;</span>parsing file: <span class="pl-c1">%s</span><span class="pl-pds">&quot;</span></span> <span class="pl-k">%</span> fname)</td>
      </tr>
      <tr>
        <td id="L770" class="blob-num js-line-number" data-line-number="770"></td>
        <td id="LC770" class="blob-code blob-code-inner js-file-line">            p <span class="pl-k">=</span> Doxy2SWIG(fname,</td>
      </tr>
      <tr>
        <td id="L771" class="blob-num js-line-number" data-line-number="771"></td>
        <td id="LC771" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">with_function_signature</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.with_function_signature,</td>
      </tr>
      <tr>
        <td id="L772" class="blob-num js-line-number" data-line-number="772"></td>
        <td id="LC772" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">with_type_info</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.with_type_info,</td>
      </tr>
      <tr>
        <td id="L773" class="blob-num js-line-number" data-line-number="773"></td>
        <td id="LC773" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">with_constructor_list</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.with_constructor_list,</td>
      </tr>
      <tr>
        <td id="L774" class="blob-num js-line-number" data-line-number="774"></td>
        <td id="LC774" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">with_attribute_list</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.with_attribute_list,</td>
      </tr>
      <tr>
        <td id="L775" class="blob-num js-line-number" data-line-number="775"></td>
        <td id="LC775" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">with_overloaded_functions</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.with_overloaded_functions,</td>
      </tr>
      <tr>
        <td id="L776" class="blob-num js-line-number" data-line-number="776"></td>
        <td id="LC776" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">textwidth</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.textwidth,</td>
      </tr>
      <tr>
        <td id="L777" class="blob-num js-line-number" data-line-number="777"></td>
        <td id="LC777" class="blob-code blob-code-inner js-file-line">                          <span class="pl-smi">quiet</span> <span class="pl-k">=</span> <span class="pl-v">self</span>.quiet)</td>
      </tr>
      <tr>
        <td id="L778" class="blob-num js-line-number" data-line-number="778"></td>
        <td id="LC778" class="blob-code blob-code-inner js-file-line">            p.generate()</td>
      </tr>
      <tr>
        <td id="L779" class="blob-num js-line-number" data-line-number="779"></td>
        <td id="LC779" class="blob-code blob-code-inner js-file-line">            <span class="pl-v">self</span>.pieces.extend(p.pieces)</td>
      </tr>
      <tr>
        <td id="L780" class="blob-num js-line-number" data-line-number="780"></td>
        <td id="LC780" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L781" class="blob-num js-line-number" data-line-number="781"></td>
        <td id="LC781" class="blob-code blob-code-inner js-file-line"><span class="pl-c"># MARK: main</span></td>
      </tr>
      <tr>
        <td id="L782" class="blob-num js-line-number" data-line-number="782"></td>
        <td id="LC782" class="blob-code blob-code-inner js-file-line"><span class="pl-k">def</span> <span class="pl-en">main</span>():</td>
      </tr>
      <tr>
        <td id="L783" class="blob-num js-line-number" data-line-number="783"></td>
        <td id="LC783" class="blob-code blob-code-inner js-file-line">    usage <span class="pl-k">=</span> <span class="pl-c1">__doc__</span></td>
      </tr>
      <tr>
        <td id="L784" class="blob-num js-line-number" data-line-number="784"></td>
        <td id="LC784" class="blob-code blob-code-inner js-file-line">    parser <span class="pl-k">=</span> optparse.OptionParser(usage)</td>
      </tr>
      <tr>
        <td id="L785" class="blob-num js-line-number" data-line-number="785"></td>
        <td id="LC785" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-f<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--function-signature<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L786" class="blob-num js-line-number" data-line-number="786"></td>
        <td id="LC786" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L787" class="blob-num js-line-number" data-line-number="787"></td>
        <td id="LC787" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L788" class="blob-num js-line-number" data-line-number="788"></td>
        <td id="LC788" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>f<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L789" class="blob-num js-line-number" data-line-number="789"></td>
        <td id="LC789" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>include function signature in the documentation. This is handy when not using swig auto-generated function definitions <span class="pl-c1">%f</span>eature(&quot;autodoc&quot;, [0,1])<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L790" class="blob-num js-line-number" data-line-number="790"></td>
        <td id="LC790" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-t<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--type-info<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L791" class="blob-num js-line-number" data-line-number="791"></td>
        <td id="LC791" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L792" class="blob-num js-line-number" data-line-number="792"></td>
        <td id="LC792" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L793" class="blob-num js-line-number" data-line-number="793"></td>
        <td id="LC793" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>t<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L794" class="blob-num js-line-number" data-line-number="794"></td>
        <td id="LC794" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>include type information for arguments in function signatures. This is similar to swig autodoc level 1<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L795" class="blob-num js-line-number" data-line-number="795"></td>
        <td id="LC795" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-c<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--constructor-list<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L796" class="blob-num js-line-number" data-line-number="796"></td>
        <td id="LC796" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L797" class="blob-num js-line-number" data-line-number="797"></td>
        <td id="LC797" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L798" class="blob-num js-line-number" data-line-number="798"></td>
        <td id="LC798" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>c<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L799" class="blob-num js-line-number" data-line-number="799"></td>
        <td id="LC799" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>generate a constructor list for class documentation. Useful for target languages where the object construction should be documented in the class documentation.<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L800" class="blob-num js-line-number" data-line-number="800"></td>
        <td id="LC800" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-a<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--attribute-list<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L801" class="blob-num js-line-number" data-line-number="801"></td>
        <td id="LC801" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L802" class="blob-num js-line-number" data-line-number="802"></td>
        <td id="LC802" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L803" class="blob-num js-line-number" data-line-number="803"></td>
        <td id="LC803" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>a<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L804" class="blob-num js-line-number" data-line-number="804"></td>
        <td id="LC804" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>generate an attributes list for class documentation. Useful for target languages where class attributes should be documented in the class documentation.<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L805" class="blob-num js-line-number" data-line-number="805"></td>
        <td id="LC805" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-o<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--overloaded-functions<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L806" class="blob-num js-line-number" data-line-number="806"></td>
        <td id="LC806" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L807" class="blob-num js-line-number" data-line-number="807"></td>
        <td id="LC807" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L808" class="blob-num js-line-number" data-line-number="808"></td>
        <td id="LC808" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>o<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L809" class="blob-num js-line-number" data-line-number="809"></td>
        <td id="LC809" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>collect all documentation for overloaded functions. Useful for target languages that have no concept of overloaded functions, but also to avoid having to attach the correct docstring to each function overload manually<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L810" class="blob-num js-line-number" data-line-number="810"></td>
        <td id="LC810" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-w<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--width<span class="pl-pds">&#39;</span></span>, <span class="pl-smi">type</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&quot;</span>int<span class="pl-pds">&quot;</span></span>,</td>
      </tr>
      <tr>
        <td id="L811" class="blob-num js-line-number" data-line-number="811"></td>
        <td id="LC811" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L812" class="blob-num js-line-number" data-line-number="812"></td>
        <td id="LC812" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>w<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L813" class="blob-num js-line-number" data-line-number="813"></td>
        <td id="LC813" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">80</span>,</td>
      </tr>
      <tr>
        <td id="L814" class="blob-num js-line-number" data-line-number="814"></td>
        <td id="LC814" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>textwidth for wrapping (default: 80). Note that the generated lines may include 2 additional spaces (for markdown).<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L815" class="blob-num js-line-number" data-line-number="815"></td>
        <td id="LC815" class="blob-code blob-code-inner js-file-line">    parser.add_option(<span class="pl-s"><span class="pl-pds">&quot;</span>-q<span class="pl-pds">&quot;</span></span>, <span class="pl-s"><span class="pl-pds">&#39;</span>--quiet<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L816" class="blob-num js-line-number" data-line-number="816"></td>
        <td id="LC816" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">action</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>store_true<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L817" class="blob-num js-line-number" data-line-number="817"></td>
        <td id="LC817" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">default</span><span class="pl-k">=</span><span class="pl-c1">False</span>,</td>
      </tr>
      <tr>
        <td id="L818" class="blob-num js-line-number" data-line-number="818"></td>
        <td id="LC818" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">dest</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>q<span class="pl-pds">&#39;</span></span>,</td>
      </tr>
      <tr>
        <td id="L819" class="blob-num js-line-number" data-line-number="819"></td>
        <td id="LC819" class="blob-code blob-code-inner js-file-line">                      <span class="pl-smi">help</span><span class="pl-k">=</span><span class="pl-s"><span class="pl-pds">&#39;</span>be quiet and minimize output<span class="pl-pds">&#39;</span></span>)</td>
      </tr>
      <tr>
        <td id="L820" class="blob-num js-line-number" data-line-number="820"></td>
        <td id="LC820" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L821" class="blob-num js-line-number" data-line-number="821"></td>
        <td id="LC821" class="blob-code blob-code-inner js-file-line">    options, args <span class="pl-k">=</span> parser.parse_args()</td>
      </tr>
      <tr>
        <td id="L822" class="blob-num js-line-number" data-line-number="822"></td>
        <td id="LC822" class="blob-code blob-code-inner js-file-line">    <span class="pl-k">if</span> <span class="pl-c1">len</span>(args) <span class="pl-k">!=</span> <span class="pl-c1">2</span>:</td>
      </tr>
      <tr>
        <td id="L823" class="blob-num js-line-number" data-line-number="823"></td>
        <td id="LC823" class="blob-code blob-code-inner js-file-line">        parser.error(<span class="pl-s"><span class="pl-pds">&quot;</span>no input and output specified<span class="pl-pds">&quot;</span></span>)</td>
      </tr>
      <tr>
        <td id="L824" class="blob-num js-line-number" data-line-number="824"></td>
        <td id="LC824" class="blob-code blob-code-inner js-file-line">    </td>
      </tr>
      <tr>
        <td id="L825" class="blob-num js-line-number" data-line-number="825"></td>
        <td id="LC825" class="blob-code blob-code-inner js-file-line">    p <span class="pl-k">=</span> Doxy2SWIG(args[<span class="pl-c1">0</span>],</td>
      </tr>
      <tr>
        <td id="L826" class="blob-num js-line-number" data-line-number="826"></td>
        <td id="LC826" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">with_function_signature</span> <span class="pl-k">=</span> options.f,</td>
      </tr>
      <tr>
        <td id="L827" class="blob-num js-line-number" data-line-number="827"></td>
        <td id="LC827" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">with_type_info</span> <span class="pl-k">=</span> options.t,</td>
      </tr>
      <tr>
        <td id="L828" class="blob-num js-line-number" data-line-number="828"></td>
        <td id="LC828" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">with_constructor_list</span> <span class="pl-k">=</span> options.c,</td>
      </tr>
      <tr>
        <td id="L829" class="blob-num js-line-number" data-line-number="829"></td>
        <td id="LC829" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">with_attribute_list</span> <span class="pl-k">=</span> options.a,</td>
      </tr>
      <tr>
        <td id="L830" class="blob-num js-line-number" data-line-number="830"></td>
        <td id="LC830" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">with_overloaded_functions</span> <span class="pl-k">=</span> options.o,</td>
      </tr>
      <tr>
        <td id="L831" class="blob-num js-line-number" data-line-number="831"></td>
        <td id="LC831" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">textwidth</span> <span class="pl-k">=</span> options.w,</td>
      </tr>
      <tr>
        <td id="L832" class="blob-num js-line-number" data-line-number="832"></td>
        <td id="LC832" class="blob-code blob-code-inner js-file-line">                  <span class="pl-smi">quiet</span> <span class="pl-k">=</span> options.q)</td>
      </tr>
      <tr>
        <td id="L833" class="blob-num js-line-number" data-line-number="833"></td>
        <td id="LC833" class="blob-code blob-code-inner js-file-line">    p.generate()</td>
      </tr>
      <tr>
        <td id="L834" class="blob-num js-line-number" data-line-number="834"></td>
        <td id="LC834" class="blob-code blob-code-inner js-file-line">    p.write(args[<span class="pl-c1">1</span>])</td>
      </tr>
      <tr>
        <td id="L835" class="blob-num js-line-number" data-line-number="835"></td>
        <td id="LC835" class="blob-code blob-code-inner js-file-line">
</td>
      </tr>
      <tr>
        <td id="L836" class="blob-num js-line-number" data-line-number="836"></td>
        <td id="LC836" class="blob-code blob-code-inner js-file-line"><span class="pl-k">if</span> <span class="pl-c1">__name__</span> <span class="pl-k">==</span> <span class="pl-s"><span class="pl-pds">&#39;</span>__main__<span class="pl-pds">&#39;</span></span>:</td>
      </tr>
      <tr>
        <td id="L837" class="blob-num js-line-number" data-line-number="837"></td>
        <td id="LC837" class="blob-code blob-code-inner js-file-line">    main()</td>
      </tr>
</table>

  </div>

</div>

<a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" style="display:none">Jump to Line</a>
<div id="jump-to-line" style="display:none">
  <!-- </textarea> --><!-- '"` --><form accept-charset="UTF-8" action="" class="js-jump-to-line-form" method="get"><div style="margin:0;padding:0;display:inline"><input name="utf8" type="hidden" value="&#x2713;" /></div>
    <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;" aria-label="Jump to line" autofocus>
    <button type="submit" class="btn">Go</button>
</form></div>

        </div>
      </div>
      <div class="modal-backdrop"></div>
    </div>
  </div>


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer" role="contentinfo">
    <ul class="site-footer-links right">
        <li><a href="https://status.github.com/" data-ga-click="Footer, go to status, text:status">Status</a></li>
      <li><a href="https://developer.github.com" data-ga-click="Footer, go to api, text:api">API</a></li>
      <li><a href="https://training.github.com" data-ga-click="Footer, go to training, text:training">Training</a></li>
      <li><a href="https://shop.github.com" data-ga-click="Footer, go to shop, text:shop">Shop</a></li>
        <li><a href="https://github.com/blog" data-ga-click="Footer, go to blog, text:blog">Blog</a></li>
        <li><a href="https://github.com/about" data-ga-click="Footer, go to about, text:about">About</a></li>
        <li><a href="https://github.com/pricing" data-ga-click="Footer, go to pricing, text:pricing">Pricing</a></li>

    </ul>

    <a href="https://github.com" aria-label="Homepage">
      <span class="mega-octicon octicon-mark-github" title="GitHub"></span>
</a>
    <ul class="site-footer-links">
      <li>&copy; 2015 <span title="0.06183s from github-fe129-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
        <li><a href="https://github.com/site/terms" data-ga-click="Footer, go to terms, text:terms">Terms</a></li>
        <li><a href="https://github.com/site/privacy" data-ga-click="Footer, go to privacy, text:privacy">Privacy</a></li>
        <li><a href="https://github.com/security" data-ga-click="Footer, go to security, text:security">Security</a></li>
        <li><a href="https://github.com/contact" data-ga-click="Footer, go to contact, text:contact">Contact</a></li>
        <li><a href="https://help.github.com" data-ga-click="Footer, go to help, text:help">Help</a></li>
    </ul>
  </div>
</div>


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-suggester-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="fullscreen-contents js-fullscreen-contents" placeholder="" aria-label=""></textarea>
      <div class="suggester-container">
        <div class="suggester fullscreen-suggester js-suggester js-navigation-container"></div>
      </div>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped tooltipped-w" aria-label="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped tooltipped-w"
      aria-label="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    
    

    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <button type="button" class="flash-close js-flash-close js-ajax-error-dismiss" aria-label="Dismiss error">
        <span class="octicon octicon-x"></span>
      </button>
      Something went wrong with that request. Please try again.
    </div>


      <script crossorigin="anonymous" src="https://assets-cdn.github.com/assets/frameworks-839820b25a65d14c34b3bba3c141cc4f0ccb09420e392bc9694fca32d00e34b1.js"></script>
      <script async="async" crossorigin="anonymous" src="https://assets-cdn.github.com/assets/github-2a900a896c75854637bbe94bcc0823b69c31d14df4f4cfaf7821ec00be94ada3.js"></script>
      
      
    <div class="js-stale-session-flash stale-session-flash flash flash-warn flash-banner hidden">
      <span class="octicon octicon-alert"></span>
      <span class="signed-in-tab-flash">You signed in with another tab or window. <a href="">Reload</a> to refresh your session.</span>
      <span class="signed-out-tab-flash">You signed out in another tab or window. <a href="">Reload</a> to refresh your session.</span>
    </div>
  </body>
</html>

