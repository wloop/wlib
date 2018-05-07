# WLib deploy instructions
WLib is hosted on two repositories under the `waterloop` organisation. The development repo `waterloop-wlib` contains 
the unit tests, various build, test, and deploy files, and scripts. The `travis` build on the `master` branch
cleans the branch and deploys to the branch `library` and the doxygen documentation to `gh-pages`.

The deploy repo `wlib` should be used in projects and is reachable by `wcosa`. WLib is deployed manually when
critical features/fixes are added, after several milestones, or periodically. WLib is deploy by pushed the development
`library` branch to the deploy repo and tagging a new release. Tags should be pushed to both repos.

```bash
git remote add devel  https://github.com/waterloop/waterloop-wlib.git
git remote add deploy https://github.com/waterloop/wlib.git
git fetch devel
git fetch deploy

git checkout devel/library
git push deploy library

git tag x.x.x
git push devel deploy --tags

```

