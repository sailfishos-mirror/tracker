import view from './editor';
import { getPrefixes, run } from './run';
import './style.scss';
import './assets/favicon.ico';

const endpoint = "/sparql";
document.getElementById("runBtn")?.addEventListener("click", async () => {
    let { result, vars } = await run(String(view.state.doc), endpoint);

    // fill results section with error/results table etc.
    document.getElementById("right").replaceChildren(...result);

    // update varaible checkbox area to allow show/hide of variables in results table
    if (vars) document.getElementById("variable-selects").replaceChildren(...vars);
    else document.getElementById("variable-selects").innerHTML = "No variables in current query, use this to show or hide variables when there are.";
});

window.addEventListener("load", async () => {
    await getPrefixes(endpoint);
});
